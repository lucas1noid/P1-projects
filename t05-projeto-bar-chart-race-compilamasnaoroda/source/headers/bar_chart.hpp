#pragma once
#include <vector>
#include <string>
#include <memory>

//movi prak
class BarItem{
    public:
    std::string label;
    long value;
    std::string category;

    BarItem(std::string l, long v, std::string c)
        : label(l), value(v), category(c){}

};

class BarChart{
    std::vector<BarItem> single_Chart; /// representa um único "frame"
    std::string moment;

    public:
    BarChart(std::string moment);
    void sort_bars();
    void add_item(BarItem item);

    //copia do vetor de items
    const std::vector<BarItem>& get_items() const;

    //retorna o ano
    const std::string& get_moment() const;
};

class Database{
    private:
    std::vector<std::shared_ptr<BarChart>> databank;
    std::string title, font;
    std::vector<std::string> m_header_lines;

    public:
    Database();
    
    void add_chart(std::shared_ptr<BarChart> chart);

    int get_total_frames() const;
    std::shared_ptr<BarChart> get_frame(int frame_index);

    //metodos para o cabeçalho do arquivo
    void set_header_lines(const std::vector<std::string>& lines);
    const std::vector<std::string>& get_header_lines() const;
};