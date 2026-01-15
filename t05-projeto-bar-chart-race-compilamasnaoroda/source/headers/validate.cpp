#include "validate.hpp"
#include "bar_chart.hpp"
#include "trim.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath> //std::round

/*!
 * Parses an .ini configuration file into a map structure.
 * 
 * @param iniFileName The file path of the .ini file to parse.
 * 
 * @return IniMap A nested map, where the outer key is the section name and the inner map holds the key-value pairs for that section. 
 * Returns an empty map if the file cannot be opened.
 */
IniMap parseIni(const std::string& iniFileName){
    IniMap configMap;
    std::ifstream archive(iniFileName);
    std::string line;
    std::string currentSection = "";

    if (!archive.is_open()) {
        return configMap; //retorna mapa vazio
    }

    while (std::getline(archive, line))
    {
        line = trim(line);//limpa espaços em branco no início/fim
        if(line.empty() || line[0] == ';' || line[0] == '#'){
            continue;//ignora linhas que não forem úteis
        }

        if (line[0] == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.length() - 2);
            configMap[currentSection]; ///separa seções do .ini
        }
        else {
            size_t posEqual = line.find('=');
            if (posEqual != std::string::npos) {
                std::string key = line.substr(0, posEqual);
                std::string value = line.substr(posEqual + 1);
                configMap[currentSection][trim(key)] = trim(value);
            }
        }
    }
    archive.close();
    return configMap;
}

/*!
 * Helper function to split a delimited string of numbers into a vector of integers.
 * 
 * @param s The input string containing delimit indices.
 * @param delimiter The character used to separate the indices (',').
 * 
 * @return std::vector<int> A vector of integers parsed from the string.
 * 
 * @note if any part of the string cannot be converted to an integer (std::stoi fails), a warning is printed to terminal and that part is skipped.
 */
std::vector<int> split_indices(const std::string& s, char delimiter) {
    std::vector<int> indices;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        try {
            indices.push_back(std::stoi(trim(item)));
        } catch (const std::exception& e) {
            std::cerr << "Warning: Ignoring invalid label index: '" << item << "'" << std::endl;
        }
    }
    return indices;
}

/*!
 * Reads a data file, processes its contents according to .ini configurations, and fill a Database.
 * 
 * @param fileName The file path of the data file to read.
 * @param db A reference to the Database object that will be filled with BC frames.
 * @param config A constant reference to the IniMap (from parseIni) containing parsing and processing rules 
 * from the [file_format], [columns], and [data_processing] sections.
 * 
 * @return true if the file was opened and processing was initiated (even if line-specific warnings occurred). 
 * false if a fatal error occurred (file not found, missing essential configuration key).
 * 
 * @note Prints warnings with std::cerr for non-fatal errors, like incorrect column counts in a row, empty values, or non-numeric values. These problematic lines are skipped.
 */
bool readFile(const std::string& fileName, Database& db, const IniMap& config) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error to open file: " << fileName << std::endl;
        return false;
    }

    //vars do .ini
    int skip_lines;
    char delimiter;
    size_t time_col_idx, value_col_idx, category_col_idx;
    std::vector<int> label_indices;//indices do .ini
    std::string label_separator;
    double value_multiplier;
    size_t max_col_index = 0;//verifica se tem colunas suficientes

    try {
        //[file_format]
        skip_lines = std::stoi(config.at("file_format").at("skip_lines"));
        std::string delim_str = config.at("file_format").at("delimiter");
        delimiter = delim_str.empty() ? ',' : delim_str[0];
        // bool has_header = (config.at("file_format").at("has_header") == "true"); //não usado por enquanto, mas seria no expectancy

        //[columns]
        time_col_idx = std::stoi(config.at("columns").at("time"));
        value_col_idx = std::stoi(config.at("columns").at("value"));
        category_col_idx = std::stoi(config.at("columns").at("category"));
        std::string label_indices_str = config.at("columns").at("label_combine_indices");
        label_separator = config.at("columns").at("label_combine_separator");
        //remove aspas do separador, se houver
        if (label_separator.front() == '"' && label_separator.back() == '"' && label_separator.length() >= 2) {
            label_separator = label_separator.substr(1, label_separator.length() - 2);
        }

        label_indices = split_indices(label_indices_str, ',');
        if (label_indices.empty()) {
            std::cerr << "Erro: 'label_combine_indices' está vazio ou inválido no .ini." << std::endl;
            return false;
        }

        value_multiplier = std::stod(config.at("data_processing").at("value_multiplier"));

        //encontrar o maior índice necessário para checagem de segurança
        max_col_index = std::max({time_col_idx, value_col_idx, category_col_idx});
        for (int idx : label_indices) {
            if ((size_t)idx > max_col_index) max_col_index = idx;
        }

    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Missing or malformatted configuration key in parameters.ini." << e.what() << std::endl;
        file.close();
        return false;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid configuration value in parameters.ini. " << e.what() << std::endl;
        file.close();
        return false;
    }


    std::string line;
    std::map<std::string, std::vector<BarItem>> data_by_year; //agrupa dados por ano/moment
    
    std::vector<std::string> header_lines;
    
    //metadados
    for (int i = 0; i < skip_lines; ++i) {
        if (!std::getline(file, line)) {
            std::cerr << "Error: File ended unexpectedly while skipping lines." << std::endl;
            return false;
        }
        std::string trimmed_line = trim(line);
        if (!trimmed_line.empty()) { //salva apenas se não estiver vazia
            header_lines.push_back(trimmed_line);
        }
    }
    db.set_header_lines(header_lines); //salva cabeçalho
    
    int line_number = skip_lines;
    while (std::getline(file, line)) {
        line_number++;
        line = trim(line);
        if (line.empty()) continue; //linhas em branco

        std::stringstream data_ss(line);
        std::string cell;
        std::vector<std::string> cells;

        while (std::getline(data_ss, cell, delimiter)) {
            cells.push_back(trim(cell));
        }

        
        //verificar se a linha tem colunas suficientes
        if (cells.size() <= max_col_index) {
            //se tiver apenas 1 célula, provavelmente é a linha indicando a contagem de registros do frame, se nao:
            if (cells.size() > 1) {
                 std::cerr << "Warning: Incorrect number of columns in row " << line_number
                          << ". Expected at least " << (max_col_index + 1) << ", Found: " << cells.size()
                          << ". Ignored line: \"" << line << "\"" << std::endl;
            }
            continue;
        }

        //extrair os dados das colunas corretas baseado nos índices do .ini
        try {
            std::string year_str = cells[time_col_idx];
            std::string value_str = cells[value_col_idx];
            std::string category_str = cells[category_col_idx];

            //construir o rótulo combinado
            std::string label_str = "";
            for (size_t i = 0; i < label_indices.size(); ++i) {
                label_str += cells[label_indices[i]];
                if (i < label_indices.size() - 1) {
                    label_str += label_separator;
                }
            }

            //tratar valores vazios
            if (value_str.empty()) {
                 std::cerr << "Warning: Empty value in line " << line_number
                           << " for the item '" << label_str << "'. Item ignored." << std::endl;
                 continue;
            }

            double value_double;
            try {
                 value_double = std::stod(value_str);
            } catch (const std::exception& e) {
                 std::cerr << "Warning: Invalid data '" << value_str << "' in the line " << line_number
                           << " for the item '" << label_str << "'. Item ignored." << std::endl;
                continue;
            }

            //aplicar multiplicador e converter para long
            long value_long = static_cast<long>(std::round(value_double * value_multiplier));

            //adicionar ao mapa temporário, agrupado por ano/momento
            data_by_year[year_str].emplace_back(label_str, value_long, category_str);

        } catch (const std::exception& e) {
            std::cerr << "Error processing line " << line_number << ": " << e.what() << ". Line: \"" << line << "\"" << std::endl;
            continue; //ignora a linha com erro e continua
        }
    }

    file.close();

    //cria os BarCharts a partir dos dados recolhidosagrupados
    for (const auto& pair : data_by_year) {
        const std::string& year = pair.first;
        const std::vector<BarItem>& items = pair.second;

        auto barChartFrame = std::make_shared<BarChart>(year); //ano como 'moment'

        for (const auto& item : items) {
            barChartFrame->add_item(item);
        }

        //ordena as barras decescentemente antes de adicionar ao DB
        barChartFrame->sort_bars();

        db.add_chart(barChartFrame);//add frame completo 
    }

    std::cout << "File '" << fileName << "' read and processed with success. "
              << db.get_total_frames() << " frames loaded." << std::endl;
    return true;
}