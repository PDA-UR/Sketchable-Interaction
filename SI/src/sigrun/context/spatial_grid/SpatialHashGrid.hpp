

#ifndef SITEST_SPATIALHASHGRID_HPP
#define SITEST_SPATIALHASHGRID_HPP

#include <sigrun/SIObject.hpp>
#include <sigrun/region/Region.hpp>

class SpatialHashGrid: public SIObject
{ SIGRUN
public:
    SpatialHashGrid(int width, int height, int cells_per_row, int cells_per_column);
    ~SpatialHashGrid();

    void update_region(Region* r);
    void register_region(Region* r);

    bool has_shared_cell(Region* a, Region* b);

private:
    int cell_index(int x, int y);
    void _register(std::vector<int>& nodes, glm::ivec4& bounds, int x, int y, int w, int h);
    void _update(std::vector<int>& nodes, glm::ivec4& bounds, int x, int y, int w, int h);

    int d_width;
    int d_height;
    int d_cells_per_row;
    int d_cells_per_column;
    int d_cell_width;
    int d_cell_height;
};


#endif //SITEST_SPATIALHASHGRID_HPP
