//
//  spn.cpp
//  corrstat
//
//  Created by Иван Дмитриевский on 23/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include "spn.h"
#include "spn/api.h"
#include "spn/str.h"
#include "session.h"

std::vector<SpnExtFunc> cst::spn::_ext_fns = std::vector<SpnExtFunc>();

void cst::spn::wrap_fn(const char *name, spn_ext_fn fn) {
  SpnExtFunc func = {name, fn};
  _ext_fns.push_back(func);
}

int scheduler_task_from_file(SpnValue *ret, int argc, SpnValue *argv,
                             void *ctx) {
  if (argc == 2 && spn_isstring(&argv[1])) {
    SpnString *arg = spn_stringvalue(&argv[1]);
    *ret = spn_makeweakuserinfo(cst::session::task_from_file(arg->cstr));
  }
  return 0;
}

int task_exec(SpnValue *ret, int argc, SpnValue *argv, void *ctx) {
  if (argc == 3 && spn_isweakuserinfo(&argv[1]) && spn_isstring(&argv[2])) {
    SpnString *kind = spn_stringvalue(&argv[2]);
    cst::task *task = (cst::task *)(spn_ptrvalue(&argv[1]));
    *ret = spn_makefloat(task->exec(cst::est_kind::enm(kind->cstr)));
  }
  return 0;
}

void cst::spn::load_fns() {
  wrap_fn("from_file", scheduler_task_from_file);
  wrap_fn("exec", task_exec);
  spn_ctx_addlib_cfuncs(&_ctx, "task", &_ext_fns.front(), _ext_fns.size());
}

cst::spn::spn() {
  spn_ctx_init(&_ctx);
  load_fns();
}

int cst::spn::exec(std::string script_path) {
  spn_ctx_execsrcfile(&_ctx, script_path.c_str(), 0);
  return 0;
}

cst::spn::~spn() { spn_ctx_free(&_ctx); }
