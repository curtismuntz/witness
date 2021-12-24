#pragma once

#include <string>
#include <glog/logging.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/delimited_message_util.h>
#include <optional>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <fcntl.h>
#include "witness/drivers/amcrest.pb.h"
#include <stdio.h>
#include <curl/curl.h>

namespace amcrest {

template<typename MyProtoType>
std::optional<MyProtoType> LoadFromTextProto(const std::string &fname) {
    MyProtoType my_proto;
    int fd = open(fname.c_str(), O_RDONLY);
    if (fd < 0) {
        LOG(ERROR) << fname << ": File not found.";
        return std::nullopt;
    }
    google::protobuf::io::FileInputStream fstream(fd);
    fstream.SetCloseOnDelete(true);

    if (!google::protobuf::TextFormat::Parse(&fstream, &my_proto)) {
        std::cerr << "Failed to parse file!" << std::endl;
        return std::nullopt;
    }
    return my_proto;
}


class Amcrest {
public:
    explicit Amcrest(const AmcrestConnection &pb) {
        hostname_ = pb.ip();
        port_ = std::to_string(pb.port());
        username_ = pb.username();
        password_ = pb.password();
        channel_ = std::to_string(pb.channel());
        subtype_ = std::to_string(pb.subtype());
    }
    std::string auth();
    std::string host_string();
    std::string get_photo_url();
    std::string get_authd_host_string();
    std::string get_real_time_stream_url();
  bool download_jpeg(const char* url, std::string& fname);
private:
    static size_t callbackfunction(void *ptr, size_t size, size_t nmemb, void* userdata);
    std::string username_, password_, hostname_, port_, channel_, subtype_;
};

} // namespace
