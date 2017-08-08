#!/usr/bin/env python
# encoding: utf-8


def options(opt):
    opt.load("compiler_cxx")


def configure(conf):
    conf.check_cxx(stlib="ode", stlibpath=["/usr/local/lib"])
    conf.load("compiler_cxx")


def build(bld):
    bld.program(source="src/main.cpp", target="main",
                includes=['/usr/local/include'],
                stlibpath=["/usr/local/lib"],
                stlib=["ode", "drawstuff"],
                framework=["Carbon", "AGL", "OpenGL", "GLUT"])
