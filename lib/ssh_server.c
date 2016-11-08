#include "libssh/include/libssh/libssh.h"
#include <stdlib.h>
int main()
{
  ssh_session my_ssh_session = ssh_new();
  if (my_ssh_session == NULL)
    exit(-1);
  ...
  ssh_free(my_ssh_session);
}
