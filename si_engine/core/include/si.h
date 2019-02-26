//
// Created by juergen on 21/01/19.
//

#ifndef CORE_SI_CORE_H
#define CORE_SI_CORE_H

#include "../include/engine.h"
#include <new>
#include <csignal>

namespace si
{
   class SI : public QObject
   {Q_OBJECT

   public:
       SI(int argc, char **argv, bool test=false);
       virtual ~SI() = default;

       virtual int run();
       virtual int quit();

       virtual void add_region(void *region);

       static void signal_handler(int signum);

   private:
       QApplication *p_qapp;

   private slots:
       void stop();
   };

   extern "C"
   {
       void *si_create_instance(int argc, char **argv);
       void si_delete_instance(void *instance);
       int si_run(void *instance);
       void si_add_region(void *instance, void *region);

       void *si_region_create_instance(region_callback rce, region_callback rcc, region_callback rcl, region_callback rocc, region_callback rdc);

       void si_region_set_blueprint(void *instance, void *region, int flag);
       void si_region_set_region_color(void *instance, void *region, int r, int g, int b, int a);
       void si_region_set_as_main_canvas(void *instance, void *region);
       void si_region_set_fullscreen(void *instance, void *region, int flag);
       void si_region_set_as_main_interaction_source(void *instance, void *region, int *x_values, int *y_values, int length);
       void si_region_set_region_on_enter_callback(void *instance, void *region, region_callback rc);
       void si_region_set_region_on_continuous_callback(void *instance, void *region, region_callback rc);
       void si_region_set_region_on_leave_callback(void *instance, void *region, region_callback rc);
   }
}


#endif //CORE_SI_CORE_H
