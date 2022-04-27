#ifndef __PROCESS_H_
#define __PROCESS_H_

#include "parse.h"
#include "params.h"
#include <fcntl.h>
#include "log.h"

int process(Request *, char *, int);

#endif