#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <seccomp.h>

#include "sandbox.h"
#include "utils.h"

void sandbox_init() {
  scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ERRNO(EPERM));
  if (ctx == NULL) {
    FAIL("failed to init seccomp context");
  }

  int rc;

  rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 1,
                        SCMP_CMP(0, SCMP_CMP_EQ, STDIN_FILENO));
  if (rc < 0) goto out;

  rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 1,
                        SCMP_CMP(0, SCMP_CMP_EQ, STDOUT_FILENO));
  if (rc < 0) goto out;

  rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 1,
                        SCMP_CMP(0, SCMP_CMP_EQ, STDERR_FILENO));
  if (rc < 0) goto out;

  rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit_group), 0);
  if (rc < 0) goto out;

  rc = seccomp_load(ctx);
  if (rc < 0) goto out;

out:
  if (rc < 0) {
    FAIL("failed to load seccomp filter: %s", strerror(-rc));
  }
}
