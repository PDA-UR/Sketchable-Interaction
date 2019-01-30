//
// Created by juergen on 21/01/19.
//

#ifndef CORE_SI_CORE_H
#define CORE_SI_CORE_H

#include "si_engine/engine.h"
#include <new>

namespace si
{
   class SI : public QObject
   {Q_OBJECT

   public:
       SI(int argc, char **argv);
       int run();
       void add_region(void *region);

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
       void *si_region_create_instance(region_callback rce, region_callback rcc, region_callback rcl);
   }
}


#endif //CORE_SI_CORE_H
