#!/usr/bin/python
# Copyright (c) 2011, the Dart project authors.  Please see the AUTHORS file
# for details. All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.

"""This is the entry point to create Dart APIs from the IDL database."""

import dartgenerator
import database
import logging.config
import optparse
import os
import shutil
import subprocess
import sys

_logger = logging.getLogger('dartdomgenerator')

_webkit_renames = {
    # W3C -> WebKit name conversion
    # TODO(vsm): Maybe Store these renames in the IDLs.
    'ApplicationCache': 'DOMApplicationCache',
    'BarProp': 'BarInfo',
    'DedicatedWorkerGlobalScope': 'DedicatedWorkerContext',
    'FormData': 'DOMFormData',
    'Selection': 'DOMSelection',
    'SharedWorkerGlobalScope': 'SharedWorkerContext',
    'Window': 'DOMWindow',
    'WorkerGlobalScope': 'WorkerContext'}

_webkit_renames_inverse = dict((v,k) for k, v in _webkit_renames.iteritems())

def GenerateDOM(systems, output_dir):
  # TODO(sra): Make this entry point also generate HTML.
  current_dir = os.path.dirname(__file__)

  generator = dartgenerator.DartGenerator(
      auxiliary_dir=os.path.join(current_dir, '..', 'src'),
      template_dir=os.path.join(current_dir, '..', 'templates'),
      base_package='')
  generator.LoadAuxiliary()

  common_database = database.Database(
      os.path.join(current_dir, '..', 'database'))
  common_database.Load()
  # Remove these types since they are mapped directly to dart.
  common_database.DeleteInterface('DOMStringMap')
  common_database.DeleteInterface('DOMStringList')
  generator.RenameTypes(common_database, {
      # W3C -> Dart renames
      'AbstractView': 'Window',
      'Function': 'EventListener',
      'DOMStringMap': 'Map<String, String>',
      'DOMStringList': 'List<String>',
      })
  generator.FilterMembersWithUnidentifiedTypes(common_database)
  webkit_database = common_database.Clone()
  # FIXME: get rid of _original_idl_types map in dartgenerator.py and
  # call ConvertToDartTypes before cloning.
  generator.ConvertToDartTypes(common_database)
  generator.ConvertToDartTypes(webkit_database)

  generated_output_dir = os.path.join(output_dir, 'generated')
  if os.path.exists(generated_output_dir):
    _logger.info('Cleaning output directory %s' % generated_output_dir)
    shutil.rmtree(generated_output_dir)


  # Generate Dart interfaces for the WebKit DOM.
  webkit_output_dir = generated_output_dir
  generator.FilterInterfaces(database = webkit_database,
                             or_annotations = ['WebKit', 'Dart'],
                             exclude_displaced = ['WebKit'],
                             exclude_suppressed = ['WebKit', 'Dart'])
  generator.RenameTypes(webkit_database, _webkit_renames)

  generator.Generate(database = webkit_database,
                     output_dir = webkit_output_dir,
                     lib_dir = output_dir,
                     module_source_preference = ['WebKit', 'Dart'],
                     source_filter = ['WebKit', 'Dart'],
                     super_database = common_database,
                     common_prefix = 'common',
                     super_map = _webkit_renames_inverse,
                     systems = systems)

  generator.Flush()

def main():
  parser = optparse.OptionParser()
  parser.add_option('--systems', dest='systems',
                    action='store', type='string',
                    default='frog,dummy,wrapping,htmlfrog',
                    help='Systems to generate (frog, native, dummy, '
                         'htmlfrog)')
  parser.add_option('--output-dir', dest='output_dir',
                    action='store', type='string',
                    default=None,
                    help='Directory to put the generated files')
  (options, args) = parser.parse_args()

  current_dir = os.path.dirname(__file__)
  output_dir = options.output_dir or os.path.join(current_dir, '..')
  systems = options.systems.split(',')

  logging.config.fileConfig(os.path.join(current_dir, 'logging.conf'))
  GenerateDOM(systems, output_dir)

  # Copy Frog DOM to frog/dom_frog.dart.
  if 'frog' in systems:
    _logger.info('Copy dom_frog to frog/')
    subprocess.call(['cd .. ; ../tools/copy_dart.py frog dom_frog.dart'],
                    shell=True);

  # Copy dummy DOM where dartc build expects it.
  if 'dummy' in systems:
    # TODO(sra): Make other tools pick this up directly, or do a copy_dart into
    # a specific directory.
    source = os.path.join(output_dir, 'dom_dummy.dart')
    target = os.path.join(output_dir, 'dom.dart')
    shutil.copyfile(source, target)

if __name__ == '__main__':
  sys.exit(main())
