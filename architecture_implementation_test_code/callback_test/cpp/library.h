#ifndef CPP_LIBRARY_H
#define CPP_LIBRARY_H

typedef void (*callback_function_int)(int);
typedef void (*callback_function_float)(float);
typedef void (*callback_function_string)(char *);

callback_function_int on_entry;
callback_function_float on_continous;
callback_function_string on_leave;

void set_callback(callback_function_int callback_entry, callback_function_float callback_continous, callback_function_string callback_leave)
{
    on_entry = callback_entry;
    on_continous = callback_continous;
    on_leave = callback_leave;
}

void trigger()
{
    on_entry(5);
    on_continous(6.7);
    on_leave("Hello There");
}


#endif