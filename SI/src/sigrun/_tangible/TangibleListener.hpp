

#ifndef SITEST_TANGIBLELISTENER_HPP
#define SITEST_TANGIBLELISTENER_HPP


#include "../SIObject.hpp"
#include "../lib/tuio_headers/oscpack/osc/OscPacketListener.h"
#include "../lib/tuio_headers/oscpack/ip/IpEndpointName.h"
#include "SITUIOObject.hpp"

/*
 * OCG only contains a session id (s_id).
 * Therefore, it is required to combine an ocg message with another message, which contains the same s_id and also type id t_id, user_id u_id, and component id c_id.
 * s_id allows for distinguishing multiple tracked objects to correctly assign and update their data based on user interaction.
 * In the particular case of the ocg message, SIGRun requires a sym message of the same s_id to correctly assign and update an interactive region which is assigned to a tracked contour.
 */


#define ALIVE_MESSAGE "/tuio2/alv"
#define TOKEN_MESSAGE "/tuio2/tok"
#define POINTER_MESSAGE "/tuio2/ptr"
#define BOUNDS_MESSAGE "/tuio2/bnd"
#define SYMBOL_MESSAGE "/tuio2/sym"
#define OCG_MESSAGE "/tuio2/ocg"
#define LIA_MESSAGE "/tuio2/lia"


class TangibleListener: public osc::OscPacketListener, public SIObject
{ SIGRUN

protected:
    void ProcessMessage(const osc::ReceivedMessage &m, const IpEndpointName &remoteEndpoint) override;

public:
    TangibleListener();

private:
    int d_source_width = 0;
    int d_source_height = 0;

    std::vector<osc::ReceivedMessage> d_bundle;
    std::vector<SITUIOObject*> d_current_objects;

    void process_message_bundle();

    void alive_information(std::vector<int>& out_dead);
    void erase_associated_regions_of_removed_tangibles(const std::vector<int>& dead);

    int frame_information();
    void handle_message_bundle(int f_id);

    void handle_tok_message(const osc::ReceivedMessage& m, int f_id);
    void handle_ptr_message(const osc::ReceivedMessage& m, int f_id);
    void handle_bnd_message(const osc::ReceivedMessage& m, int f_id);
    void handle_sym_message(const osc::ReceivedMessage& m, int f_id);
    void handle_ocg_message(const osc::ReceivedMessage& m, int f_id);
    void handle_lia_message(const osc::ReceivedMessage& m, int f_id);

    SITUIOObject* current_object(int s_id);
};


#endif //SITEST_TANGIBLELISTENER_HPP
