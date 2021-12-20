#include "amcrest.h"

namespace amcrest {

std::string Amcrest::auth() {
    std::stringstream ss;
    ss << username_ << ":" << password_;
    return ss.str();
}
std::string Amcrest::host_string() {
    std::stringstream ss;
    ss << hostname_ << ":" << port_;
    return ss.str();
}
std::string Amcrest::get_authd_host_string() {
    std::stringstream ss;
    ss << auth() << "@" << host_string();
    return ss.str();
}
std::string Amcrest::get_real_time_stream_url() {
    std::stringstream ss;
    ss << "rtsp://" << get_authd_host_string() << "/cam/realmonitor?channel=" << channel_ << "&subtype=" << subtype_;
    return ss.str();
}

} // namspace
