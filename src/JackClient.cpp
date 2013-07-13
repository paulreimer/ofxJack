#include "JackClient.h"
#include <iostream>

//--------------------------------------------------------------
JackClient::JackClient(std::string _name)
: clientImpl(NULL)
, name(_name)
, sampleRate(0)
, maxBufferSize(0)
{}

//--------------------------------------------------------------
JackClient::~JackClient()
{
  if (clientImpl)
  {
    jack_client_close(clientImpl);
    clientImpl = NULL;
  }
}

//--------------------------------------------------------------
void
JackClient::setup(std::string _name)
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
    std::cerr << "jack server not running?" << std::endl;
}

//--------------------------------------------------------------
void
JackClient::activate()
{
  if ((clientImpl!=NULL) && (jack_activate(clientImpl)==0))
  {}
  else
    std::cerr << "cannot activate JACK clientImpl" << std::endl;
}
//--------------------------------------------------------------
int
JackClient::onJackBufferSizeChanged(jack_nframes_t nframes)
{
  std::cout << "Maximum buffer size: " << nframes << std::endl;
  maxBufferSize = nframes;
	return 0;
}

//--------------------------------------------------------------
int
JackClient::onJackSampleRateChanged(jack_nframes_t nframes)
{
  std::cout << "Sample rate: " << nframes << "/sec" << std::endl;
  sampleRate = nframes;
  return 0;
}

//--------------------------------------------------------------
void
JackClient::onJackShutdown()
{
  //	exit(1);
  std::cout << "onJackShutdown(): lost connection, will probably crash..." << std::endl;
}

static int
_onJackProcess(jack_nframes_t nframes, void *arg)
{
  JackClient* client = (JackClient*)arg;
  if (client)
    client->onJackProcess(nframes);
}

static int
_onJackBufferSizeChanged(jack_nframes_t nframes, void *arg)
{
  JackClient* client = (JackClient*)arg;
  if (client)
    client->onJackBufferSizeChanged(nframes);
}

static int
_onJackSampleRateChanged(jack_nframes_t nframes, void *arg)
{
  JackClient* client = (JackClient*)arg;
  if (client)
    client->onJackSampleRateChanged(nframes);
}

static void
_onJackShutdown(void *arg)
{
  JackClient* client = (JackClient*)arg;
  if (client)
    client->onJackShutdown();
}
