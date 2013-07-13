#pragma once

extern "C" {
#include <jack/jack.h>
}

#include <string>

class JackClient
{
public:
  JackClient(std::string _name="OpenLaseClient");
  virtual ~JackClient();
  
  void setup(std::string _name="");
  void activate();

  virtual int onJackProcess(jack_nframes_t nframes) =0;
  virtual int onJackBufferSizeChanged(jack_nframes_t nframes);
  virtual int onJackSampleRateChanged(jack_nframes_t nframes);
  virtual void onJackShutdown();

protected:
  std::string name;
  jack_client_t *clientImpl;
  
  float sampleRate;
  size_t maxBufferSize;

private:
  
};

extern "C" {
  static int _onJackProcess(jack_nframes_t nframes, void *arg);
  static int _onJackBufferSizeChanged(jack_nframes_t nframes, void *arg);
  static int _onJackSampleRateChanged(jack_nframes_t nframes, void *arg);
  static void _onJackShutdown(void *arg);
}
