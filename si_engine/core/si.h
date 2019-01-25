//
// Created by juergen on 21/01/19.
//

#ifndef CORE_SI_CORE_H
#define CORE_SI_CORE_H

#include "si_engine/engine.h"
#include <new>
#include "si_plugin/plugin.h"

namespace si
{
   class SI : public QObject
   {Q_OBJECT

   public:
       SI(int argc, char **argv);
       int run();
       void register_plugin(void *plugin);

   private:
       QApplication *p_qapp;

   private slots:
       void stop();
   };

   extern "C"
   {
       void *si_create_instance(int argc, char **argv);
       void si_delete_instance(void *instance);
       int si_run(void * instance);
       int si_register_plugin(void *instance, void *plugin);
   }
}


#endif //CORE_SI_CORE_H
