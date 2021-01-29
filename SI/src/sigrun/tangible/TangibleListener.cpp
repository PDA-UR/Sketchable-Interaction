

#include <sigrun/context/Context.hpp>
#include "TangibleListener.hpp"

TangibleListener::TangibleListener()
{

}

void TangibleListener::ProcessMessage(const osc::ReceivedMessage &m, const IpEndpointName &remoteEndpoint)
{
    d_bundle.push_back(m);

    if(!strcmp(m.AddressPattern(), ALIVE_MESSAGE))
    {
        std::mutex mutex;
        std::lock_guard<std::mutex> guard(mutex);

        process_message_bundle();
        d_bundle.clear();

        for(SITUIOObject* tobj: d_current_objects)
            Context::SIContext()->tangible_manager()->update(tobj);
    }
}

void TangibleListener::process_message_bundle()
{
    std::vector<int> dead;

    alive_information(dead);
    erase_associated_regions_of_removed_tangibles(dead);
    handle_message_bundle(frame_information());
}

void TangibleListener::alive_information(std::vector<int> &out_dead)
{
    std::vector<int> alive;

    for(auto it = d_bundle.back().ArgumentsBegin(); it != d_bundle.back().ArgumentsEnd(); ++it)
        alive.push_back(it->AsInt32());

    for(SITUIOObject* tobj: d_current_objects)
    {
        if(std::find_if(alive.begin(), alive.end(), [&tobj](int a)
        {
            return tobj->s_id() == a;
        }) == alive.end())
            out_dead.push_back(tobj->s_id());
    }
}

void TangibleListener::erase_associated_regions_of_removed_tangibles(const std::vector<int> &dead)
{
    for (int i = 0; i < dead.size(); ++i)
    {
        d_current_objects.erase(std::remove_if(d_current_objects.begin(), d_current_objects.end(), [&](SITUIOObject *tobj)
        {
            if (tobj->s_id() == dead[i])
            {
                Context::SIContext()->tangible_manager()->remove(tobj->s_id());

                delete tobj;
                tobj = nullptr;

                return true;
            }

            return false;
        }), d_current_objects.end());
    }
}

int TangibleListener::frame_information()
{
    int f_id, dim;
    osc::TimeTag time;
    const char* source;

    d_bundle.front().ArgumentStream() >> f_id >> time >> dim >> source >> osc::EndMessage;

    return f_id;
}

void TangibleListener::handle_message_bundle(int f_id)
{
    for(auto it = d_bundle.begin() + 1; it != d_bundle.end() - 1; ++it)
    {
        try
        {
            if(!strcmp(it->AddressPattern(), TOKEN_MESSAGE))
                handle_tok_message(*it, f_id);
            else if (!strcmp(it->AddressPattern(), POINTER_MESSAGE))
                handle_ptr_message(*it, f_id);
            else if(!strcmp(it->AddressPattern(), BOUNDS_MESSAGE))
                handle_bnd_message(*it, f_id);
            else if (!strcmp(it->AddressPattern(), SYMBOL_MESSAGE))
                handle_sym_message(*it, f_id);
            else if(!strcmp(it->AddressPattern(), OCG_MESSAGE))
                handle_ocg_message(*it, f_id);
            else
                WARN("unknown message " + std::string(it->AddressPattern()));
        }
        catch(std::exception e)
        {
            ERROR("error while parsing tuio2 message " + std::string(it->AddressPattern()) + ": " + e.what());
        }
    }
}

void TangibleListener::handle_tok_message(const osc::ReceivedMessage &m, int f_id)
{
    int s_id = m.ArgumentsBegin()->AsInt32();
    SITUIOObject* o = current_object(s_id);

    if(!o)
    {
        d_current_objects.push_back(new SITUIOObject(s_id, f_id));
        d_current_objects.back()->add_token_data(m);
    }
    else
        o->add_token_data(m);
}

void TangibleListener::handle_ptr_message(const osc::ReceivedMessage &m, int f_id)
{
    int s_id = m.ArgumentsBegin()->AsInt32();
    SITUIOObject* o = current_object(s_id);

    if(!o)
    {
        d_current_objects.push_back(new SITUIOObject(s_id, f_id));
        d_current_objects.back()->add_pointer_data(m);
    }
    else
        o->add_pointer_data(m);
}

void TangibleListener::handle_bnd_message(const osc::ReceivedMessage &m, int f_id)
{
    int s_id = m.ArgumentsBegin()->AsInt32();
    SITUIOObject* o = current_object(s_id);

    if(!o)
    {
        d_current_objects.push_back(new SITUIOObject(s_id, f_id));
        d_current_objects.back()->add_bounds_data(m);
    }
    else
        o->add_bounds_data(m);
}

void TangibleListener::handle_sym_message(const osc::ReceivedMessage &m, int f_id)
{
    int s_id = m.ArgumentsBegin()->AsInt32();
    SITUIOObject* o = current_object(s_id);

    if(!o)
    {
        d_current_objects.push_back(new SITUIOObject(s_id, f_id));
        d_current_objects.back()->add_symbol_data(m);
    }
    else
        o->add_symbol_data(m);
}

void TangibleListener::handle_ocg_message(const osc::ReceivedMessage &m, int f_id)
{
    int s_id = m.ArgumentsBegin()->AsInt32();
    SITUIOObject* o = current_object(s_id);

    if(!o)
    {
        d_current_objects.push_back(new SITUIOObject(s_id, f_id));
        d_current_objects.back()->add_outer_contour_geometry_data(m);
    }
    else
        o->add_outer_contour_geometry_data(m);
}

SITUIOObject *TangibleListener::current_object(int s_id)
{
    auto bit = std::find_if(d_current_objects.begin(), d_current_objects.end(), [&s_id](SITUIOObject* tobj)
    {
        return s_id == tobj->s_id();
    });

    return bit == d_current_objects.end() ? nullptr: *bit;
}
