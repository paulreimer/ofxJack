#include "ofxJackClient.h"
#include <iostream>

//--------------------------------------------------------------
ofxJackClient::ofxJackClient(std::string _name)
: clientImpl(NULL)
, name(_name)
, sampleRate(0)
, maxBufferSize(0)
{}

//--------------------------------------------------------------
ofxJackClient::~ofxJackClient()
{
  if (clientImpl)
  {
    jack_client_close(clientImpl);
    clientImpl = NULL;
  }
}

//--------------------------------------------------------------
void
ofxJackClient::setup(std::string _name)
{
	jack_status_t jack_status;

  if (!_name.empty())
      name = _name;
  
	if ((clientImpl = jack_client_open(name.c_str(), JackNullOption, &jack_status)) != NULL)
  {
    jack_set_process_callback(clientImpl, _onJackProcess, this);
    jack_set_buffer_size_callback(clientImpl, _onJackBufferSizeChanged, this);
    jack_set_sample_rate_callback(clientImpl, _onJackSampleRateChanged, this);
    jack_on_shutdown(clientImpl, _onJackShutdown, this);
  }
  else
    std::cerr << name << ":: jack server not running?" << std::endl;
}

//--------------------------------------------------------------
void
ofxJackClient::activate()
{
  if ((clientImpl != NULL) && (jack_activate(clientImpl) == 0))
  {}
  else
    std::cerr << name << ":: cannot activate JACK client" << std::endl;
}

//--------------------------------------------------------------
int
ofxJackClient::onJackBufferSizeChanged(jack_nframes_t nframes)
{
  std::cout << name << ":: maximum buffer size = " << nframes << std::endl;
  maxBufferSize = nframes;
	return 0;
}

//--------------------------------------------------------------
int
ofxJackClient::onJackSampleRateChanged(jack_nframes_t nframes)
{
  std::cout << name << ":: sample rate = " << nframes << "Hz" << std::endl;
  sampleRate = nframes;
  return 0;
}

//--------------------------------------------------------------
void
ofxJackClient::onJackShutdown()
{
  //	exit(1);
  std::cout << name << ", onJackShutdown(): lost connection, will probably crash..." << std::endl;
}

static int
_onJackProcess(jack_nframes_t nframes, void *arg)
{
  ofxJackClient* client = (ofxJackClient*)arg;
  if (client)
    client->onJackProcess(nframes);
}

static int
_onJackBufferSizeChanged(jack_nframes_t nframes, void *arg)
{
  ofxJackClient* client = (ofxJackClient*)arg;
  if (client)
    client->onJackBufferSizeChanged(nframes);
}

static int
_onJackSampleRateChanged(jack_nframes_t nframes, void *arg)
{
  ofxJackClient* client = (ofxJackClient*)arg;
  if (client)
    client->onJackSampleRateChanged(nframes);
}

static void
_onJackShutdown(void *arg)
{
  ofxJackClient* client = (ofxJackClient*)arg;
  if (client)
    client->onJackShutdown();
}
