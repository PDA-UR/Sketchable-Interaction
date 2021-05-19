

#include "SpatialHashGrid.hpp"

SpatialHashGrid::SpatialHashGrid(int width, int height, int cells_per_row, int cells_per_column):
    d_width(width),
    d_height(height),
    d_cells_per_row(cells_per_row),
    d_cells_per_column(cells_per_column),
    d_cell_width(d_width / d_cells_per_row),
    d_cell_height(d_height / d_cells_per_column)
{}

SpatialHashGrid::~SpatialHashGrid() = default;

void SpatialHashGrid::register_region(Region *r)
{
    _register(r->grid_nodes(), r->grid_bounds(),r->aabb()[0].x + r->transform()[0].z, r->aabb()[0].y + r->transform()[1].z, r->width(), r->height());
}

void SpatialHashGrid::update_region(Region *r)
{
    _update(r->grid_nodes(), r->grid_bounds(),r->aabb()[0].x + r->transform()[0].z, r->aabb()[0].y + r->transform()[1].z, r->width(), r->height());
}

bool SpatialHashGrid::has_shared_cell(Region *a, Region *b)
{
    const std::vector<int>& a_nodes = a->grid_nodes();
    const std::vector<int>& b_nodes = b->grid_nodes();

    return std::find_first_of(a_nodes.begin(), a_nodes.end(), b_nodes.begin(), b_nodes.end()) != a_nodes.end();
}

int SpatialHashGrid::cell_index(int x, int y)
{
    return d_cells_per_row * (y / d_cell_height) + (x / d_cell_width);
}

void SpatialHashGrid::_register(std::vector<int>& nodes, glm::ivec4& bounds, int x, int y, int w, int h)
{
    int tln = cell_index(x, y);
    int trn = cell_index(x + w, y);
    int bln = cell_index(x, y + h);
    int brn = cell_index(x + w, y + h);

    bounds = glm::ivec4(tln, bln, brn, trn);

    nodes.clear();

    for(int i = 0, xc = (trn - tln); i <= xc; ++i)
        for(int k = 0, yc = (bln - tln) / d_cells_per_row; k <= yc; ++k)
            nodes.push_back(tln + i + k * d_cells_per_row);
}

void SpatialHashGrid::_update(std::vector<int>& nodes, glm::ivec4& bounds, int x, int y, int w, int h)
{
    int tln = cell_index(x, y);
    int trn = cell_index(x + w, y);
    int bln = cell_index(x, y + h);
    int brn = cell_index(x + w, y + h);

    if(tln == bounds.x && bln == bounds.y && brn == bounds.z && trn == bounds.w)
        return;

    _register(nodes, bounds, x, y, w, h);
}
