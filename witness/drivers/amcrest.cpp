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

std::string Amcrest::get_photo_url() {
  std::stringstream ss;
  ss << "http://" << hostname_ << "/cgi-bin/snapshot.cgi?channel=" << channel_;
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

// https://stackoverflow.com/q/42336140
size_t Amcrest::callbackfunction(void *ptr, size_t size, size_t nmemb, void* userdata)
{
  FILE* stream = (FILE*)userdata;
  if (!stream) {
    LOG(ERROR) << "!!! No stream";
    return 0;
  }

  size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
  return written;
}

bool Amcrest::download_jpeg(const char* url, std::string& fname)
{
  DLOG(INFO) << "Creating file: " << fname;
  FILE* fp = fopen(fname.c_str(), "wb");
  if (!fp)
  {
    LOG(ERROR) << "!!! Failed to create file on the disk";
    return false;
  }

  CURL* curlCtx = curl_easy_init();
  curl_easy_setopt(curlCtx, CURLOPT_URL, url);
  curl_easy_setopt(curlCtx, CURLOPT_WRITEDATA, fp);
  curl_easy_setopt(curlCtx, CURLOPT_WRITEFUNCTION, callbackfunction);
  curl_easy_setopt(curlCtx, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(curlCtx, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
  std::stringstream ss;
  ss << username_ << ":" << password_;
  curl_easy_setopt(curlCtx, CURLOPT_USERPWD, ss.str().c_str());

  CURLcode rc = curl_easy_perform(curlCtx);
  if (rc)
  {
    LOG(ERROR) <<"!!! Failed to download: " << url;
    return false;
  }

  long res_code = 0;
  curl_easy_getinfo(curlCtx, CURLINFO_RESPONSE_CODE, &res_code);
  if (!((res_code == 200 || res_code == 201) && rc != CURLE_ABORTED_BY_CALLBACK))
  {
    LOG(ERROR) << "!!! Response code: " << res_code;
    return false;
  }

  curl_easy_cleanup(curlCtx);

  fclose(fp);

  return true;
}

} // namspace
