#include "bar_chart.hpp"
#include <iostream> // Para debug, se necessário
#include <algorithm>

/*!
 * Constructor for the BarChart class.
 *
 * @param moment The string representing this "moment" or frame ex:("2000").
 */
BarChart::BarChart(std::string moment) 
    : moment(moment) {
}

/*!
 * Default constructor for the Database class.
 *
 * @note Initializes default values database.
 */
Database::Database() {
    // Inicializa o título, fonte, etc., se necessário
    this->title = "Default Title";
    this->font = "DefaultFont.ttf";
}

/*!
 * Adds a new frame (BarChart) to the database.
 *
 * @param chart A 'shared_ptr' to the BC object to be added.
 */
void Database::add_chart(std::shared_ptr<BarChart> chart) {
    this->databank.push_back(chart);
}

/*!
 * Adds an item (a bar) to this frame.
 *
 * @param item The BarItem object to be added.
 */
void BarChart::add_item(BarItem item){
    this->single_Chart.push_back(item);
}

/*!
 * Gets the total number of frames in the database.
 *
 * @return The total number of frames.
 */
int Database::get_total_frames() const{
    return this->databank.size();
}

/*!
 * Gets a pointer to a specific frame.
 *
 * @param frame_index The index of the frame.
 * 
 * @return a 'shared_ptr' to the BarChart, or 'nullptr' if the index is invalid.
 */
std::shared_ptr<BarChart> Database::get_frame(int frame_index){
    if (frame_index >= 0 && (size_t)frame_index < this->databank.size())
    {
        return this->databank[frame_index];
    }
    return nullptr; //se o índice for inválido retorna um ponteiro nulo
}

/*!
 * Sorts the items (bars) within this frame in descending order of value.
 *
 * @note Uses 'std::sort' and a lambda expression for comparison.
 */
void BarChart::sort_bars() {
    //usa std::sort (do <algorithm>) com uma função lambda como comparador
    std::sort(this->single_Chart.begin(), this->single_Chart.end(), 
        [](const BarItem& a, const BarItem& b) {
            //'true' se 'a' deve vir antes de 'b' (ordem decrescente)
            return a.value > b.value;
        });
}

/*!
 * @brief Gets a constant reference to the vector of items (bars) for this frame.
 *
 * @return a 'const std::vector<BarItem>&' containing all items.
 */
const std::vector<BarItem>& BarChart::get_items() const {
    return single_Chart;
}

/*!
 * Gets the 'moment' string for the frame.
 *
 * @return a 'const std::string&' representing the moment.
 */
const std::string& BarChart::get_moment() const {
    return moment;
}

/*!
 * Stores the header lines read from the data file.
 *
 * @param lines A vector of strings containing the header lines.
 */
void Database::set_header_lines(const std::vector<std::string>& lines) {
    this->m_header_lines = lines;
}

/*!
 * Recover the stored header lines.
 *
 * @return A 'const std::vector<std::string>&' with the header lines.
 */
const std::vector<std::string>& Database::get_header_lines() const {
    return this->m_header_lines;
}