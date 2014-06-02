#!/usr/bin/python

# Copyright (c) 2014, the Dart project authors.  Please see the AUTHORS file
# for details. All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.

"""
Buildbot steps for functional testing master and slaves
"""

import os
import re
import shutil
import sys

import bot
import bot_utils

utils = bot_utils.GetUtils()

FT_BUILDER = r'ft-slave-(linux|mac)'
FT_MASTER = r'ft-master'

HOST_OS = utils.GuessOS()

EDITOR_LOCATION='/home/chrome-bot/Desktop'

def SrcConfig(name, is_buildbot):
  """Returns info for the current buildbot based on the name of the builder.

  - mode: always "release"
  - system: always "linux" or "mac"
  """
  pattern = re.match(FT_BUILDER, name)
  master_pattern = re.match(FT_MASTER, name)
  if not pattern and not master_pattern:
    return None
  if master_pattern:
    tag = 'master'
    system = 'linux'
  else:
    tag = 'slave'
    system = pattern.group(1)
  return bot.BuildInfo('none', 'none', 'release', system,
                       builder_tag=tag)

def Run(args):
  print "Running: %s" % ' '.join(args)
  sys.stdout.flush()
  bot.RunProcess(args)

def FTSlave(config):
  with bot.BuildStep('Fetching editor'):
    revision = int(os.environ['BUILDBOT_GOT_REVISION'])
    bot_name, _ = bot.GetBotName()
    print bot_name
    channel = bot_utils.GetChannelFromName(bot_name)
    namer = bot_utils.GCSNamer(channel=channel)
    system = config.system
    if system == 'mac':
      system = 'macos'
    editor_path = namer.editor_zipfilepath(revision, system, 'x64')
    gsutils = bot_utils.GSUtil()
    local_path = os.path.join(EDITOR_LOCATION, 'editor.zip')
    if os.path.exists(local_path):
      os.remove(local_path)
    local_extracted = os.path.join(EDITOR_LOCATION, 'dart')
    shutil.rmtree(local_extracted, ignore_errors=True)
    gsutils.execute(['cp', editor_path, local_path])
    Run(['unzip', local_path, '-d', EDITOR_LOCATION])

def FTMaster(config):
  run = int(os.environ['BUILDBOT_ANNOTATED_STEPS_RUN'])
  with bot.BuildStep('Master run %s' % run):
    if run == 1:
      print 'Not doing anything on master before the triggers'
      return
    else:
      builddir = os.path.join(bot_utils.DART_DIR,
                              utils.GetBuildDir(HOST_OS, HOST_OS),
                              'functional_testing')
      shutil.rmtree(builddir, ignore_errors=True)
      os.makedirs(builddir)
      script_locations = os.path.join(bot_utils.DART_DIR, 'editor', 'ft')
      Run(['/home/chrome-bot/func-test/bot-run', builddir, script_locations])

def FTSteps(config):
  if config.builder_tag == 'master':
    FTMaster(config)
  else:
    FTSlave(config)
  
if __name__ == '__main__':
  bot.RunBot(SrcConfig, FTSteps, build_step=None)
