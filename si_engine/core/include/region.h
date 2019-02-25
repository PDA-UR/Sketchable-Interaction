//
// Created by juergen on 25/01/19.
//

#ifndef CORE_REGION_H
#define CORE_REGION_H

#include <QWidget>
#include <QPointF>
#include <vector>
#include <functional>
#include <QColor>

namespace si
{
    extern "C"
    {
    typedef int (*region_callback)(long);
    }

    class region : public QWidget
    { Q_OBJECT
    public:
        region(const QPolygon &s, QWidget *parent= nullptr);
        explicit region(QWidget *parent= nullptr);
        explicit region(region_callback rce, region_callback rcc, region_callback rcl, region_callback rocc, region_callback rdc);

        region(const region &copy, QWidget *parent= nullptr);

        virtual ~region() = default;

        /* SHAPE HANDLING*/
        const QPolygon &shape() const;
        const QRect &shape_aabb() const;
        void set_shape(const QPolygon &s);
        bool has_shape();
        virtual void update_shape_coords(int x, int y);
        /* END OF SHAPE HANDLING */

        /* EFFECT FUNCTIONS HANDLING */
        void set_enter_callback(const std::function<int(long)> &callback);
        void set_enter_callback(region_callback rc);
        void set_continuous_callback(const std::function<int(long)> &callback);
        void set_continuous_callback(region_callback rc);
        void set_leave_callback(const std::function<int(long)> &callback);
        void set_leave_callback(region_callback rc);
        void set_create_callback(const std::function<int(long)> &callback);
        void set_create_callback(region_callback rc);
        void set_destroy_callback(const std::function<int(long)> &callback);
        void set_destroy_callback(region_callback rc);
        int on_enter(long uuid);
        int on_continuous(long uuid);
        int on_leave(long uuid);
        int on_create(long uuid);
        int on_destroy(long uuid);
        /* END OF EFFECT FUNCTIONS HANDLING */

        /* COLOR HANDLING */
        const QColor &region_color() const;
        void set_region_color(int r, int g, int b, int a=255);
        /* END OF COLOR HANDLING */

        /* ICON HANDLING */
        const region &icon_region() const;
        void set_icon_region(const std::string &icon_path);
        /* END OF ICON HANDLING */

        /* BLUEPRINT HANDLING */
        const bool is_blueprint() const;
        void set_blueprint(bool flag);
        /* END OF BLUEPRINT HANDLING */

        /* MAIN CANVAS HANDLING */
        void set_as_main_canvas();
        void load_default_canvas();
        void set_fullscreen(bool fullscreen=true);
        /* END OF MAIN CANVAS HANDLING */

        /* MAIN INTERACTION SOURCE HANDLING */
        void set_as_main_interaction_source();
        void load_default_interaction_source();
        /* END OF MAIN INTERACTION SOURCE HANDLING */

        /* LINKS HANDLING */
        /* END OF LINKS HANDLING */

    protected:
        /* callback functions */
        std::function<int(long)> on_region_enter;
        std::function<int(long)> on_region_continuous;
        std::function<int(long)> on_region_leave;
        std::function<int(long)> on_region_create;
        std::function<int(long)> on_region_destroy;

        /* shape */
        QPolygon d_shape;
        QRect d_shape_aabb;

        /* misc */
        bool d_is_standard_lib = false;
        std::vector<long> d_collider_uuids;

        /* color */
        QColor d_region_color = QColor(255, 255, 255);

        /* icon */
        // region icon_region = region::as_icon_region(default_icon_path);

        /* blueprint */
        bool d_is_blueprint = true;

        /* main canvas */
        bool d_is_main_canvas = false;
        bool d_is_fullscreen = true;

        /* main interaction source */
        bool d_is_main_interaction_source = false;
    };
}

#endif //CORE_REGION_H
