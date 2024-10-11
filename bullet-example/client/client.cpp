#include <iomanip>
#include <iostream>
#include <sstream>
#include <memory>

#include <condition_variable>
#include <thread>

#include <vsomeip/vsomeip.hpp>

#define SAMPLE_SERVICE_ID    0x1234
#define SAMPLE_INSTANCE_ID   0x5678
#define SAMPLE_METHOD_ID     0x0421


std::string username = "Hazem";

std::shared_ptr< vsomeip::application > app;
std::mutex mutex;
std::condition_variable condition;

void run() {

  std::unique_lock<std::mutex> its_lock(mutex);
  condition.wait(its_lock);
 std::cout << "lock is acq. " << std::endl;

  std::shared_ptr<vsomeip::message> request;
  request = vsomeip::runtime::get()->create_request();

  request->set_service(SAMPLE_SERVICE_ID);
  request->set_instance(SAMPLE_INSTANCE_ID);
  request->set_method(SAMPLE_METHOD_ID);

  std::shared_ptr< vsomeip::payload > its_payload = vsomeip::runtime::get()->create_payload();
  std::vector< vsomeip::byte_t > its_payload_data;

  std::string name = username;
  for (size_t i = 0 ; i < name.size(); ++i) {
      its_payload_data.push_back(name[i] % 256);
  }
  its_payload->set_data(its_payload_data);
  request->set_payload(its_payload);
  app->send(request);

  std::cout << "Request is sent " << std::endl;
 }

void on_message(const std::shared_ptr<vsomeip::message> &_response) {

  std::shared_ptr<vsomeip::payload> its_payload = _response->get_payload();
  vsomeip::length_t l = its_payload->get_length();

  // Get payload
  std::string ss;
  for (vsomeip::length_t i=0; i<l; i++) {
     ss.push_back( (char)*(its_payload->get_data()+i) );
  }

  std::cout << "CLIENT: Received message  [" << ss << " ]" << std::endl;
}

void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
    std::cout << "CLIENT: Service ["
            << std::setw(4) << std::setfill('0') << std::hex << _service << "." << _instance
            << "] is "
            << (_is_available ? "available." : "NOT available.")
            << std::endl;
    if (_is_available == true)
        condition.notify_one();
}

int main() {

    app = vsomeip::runtime::get()->create_application("CLIENT");
    app->init();
    app->register_availability_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, on_availability);
    app->request_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    app->register_message_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID, on_message);
    std::thread sender(run);
    app->start();
}