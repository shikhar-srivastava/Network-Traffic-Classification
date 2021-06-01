#include <libssh/libssh.h>
#include <stdlib.h>
#include <stdio.h> 
#include <libssh/server.h>
#include "libssh/examples_common.h"

int send_command_and_receive_result(ssh_session session,char* command);

int main(int argc, char const *argv[])
{
    ssh_session my_ssh_session;
    my_ssh_session = ssh_new();
  int verbosity = SSH_LOG_NOLOG;
    if (my_ssh_session == NULL)
        return -1;


    my_ssh_session = connect_ssh("localhost","user",2232,verbosity);
  if(my_ssh_session == NULL)
  {
    printf("Myssh NULL error connect_ssh\n");
    return -1;
  }
  printf("Connection Completed shell startup");
    send_command_and_receive_result(my_ssh_session,"ls");
   //shell_session(my_ssh_session);


    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
    return 0;
}

int send_command_and_receive_result(ssh_session session,char* command)
{
  ssh_channel channel;
  int rc;
  char buffer[256];
  int nbytes;
  channel = ssh_channel_new(session);
  if (channel == NULL)
    return SSH_ERROR;
  rc = ssh_channel_open_session(channel);
  if (rc != SSH_OK)
  {
    ssh_channel_free(channel);
    return rc;
  }
  rc = ssh_channel_request_exec(channel, command);
  if (rc != SSH_OK)
  {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return rc;
  }
  nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
  while (nbytes > 0)
  {
    if (write(1, buffer, nbytes) != (unsigned int) nbytes)
    {
      ssh_channel_close(channel);
      ssh_channel_free(channel);
      return SSH_ERROR;
    }
    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
  }

  if (nbytes < 0)
  {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return SSH_ERROR;
  }
  ssh_channel_send_eof(channel);
  ssh_channel_close(channel);
  ssh_channel_free(channel);
  return SSH_OK;
}


