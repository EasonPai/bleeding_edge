library pub.executable;
import 'dart:async';
import 'dart:io';
import 'package:barback/barback.dart';
import 'package:path/path.dart' as p;
import 'package:stack_trace/stack_trace.dart';
import 'barback/asset_environment.dart';
import 'entrypoint.dart';
import 'exit_codes.dart' as exit_codes;
import 'io.dart';
import 'log.dart' as log;
import 'utils.dart';
Future<int> runExecutable(Entrypoint entrypoint, String package,
    String executable, Iterable<String> args, {bool isGlobal: false}) {
  final completer0 = new Completer();
  scheduleMicrotask(() {
    try {
      join0() {
        var localSnapshotPath =
            p.join(".pub", "bin", package, "${executable}.dart.snapshot");
        join1() {
          var rootDir = "bin";
          var parts = p.split(executable);
          join2() {
            AssetEnvironment.create(
                entrypoint,
                BarbackMode.RELEASE,
                useDart2JS: false).then((x0) {
              try {
                var environment = x0;
                environment.barback.errors.listen(((error) {
                  log.error(log.red("Build error:\n$error"));
                }));
                var server;
                join3() {
                  var assetPath = "${p.url.joinAll(p.split(executable))}.dart";
                  var id = new AssetId(server.package, assetPath);
                  completer0.complete(
                      environment.barback.getAssetById(id).then(((_) {
                    final completer0 = new Completer();
                    scheduleMicrotask(() {
                      try {
                        var vmArgs = [];
                        vmArgs.add("--checked");
                        var relativePath =
                            p.url.relative(assetPath, from: p.url.joinAll(p.split(server.rootDirectory)));
                        vmArgs.add(server.url.resolve(relativePath).toString());
                        vmArgs.addAll(args);
                        Process.start(Platform.executable, vmArgs).then((x0) {
                          try {
                            var process = x0;
                            process.stderr.listen(stderr.add);
                            process.stdout.listen(stdout.add);
                            stdin.listen(process.stdin.add);
                            completer0.complete(process.exitCode);
                          } catch (e0) {
                            completer0.completeError(e0);
                          }
                        }, onError: (e1) {
                          completer0.completeError(e1);
                        });
                      } catch (e2) {
                        completer0.completeError(e2);
                      }
                    });
                    return completer0.future;
                  })).catchError(((error, stackTrace) {
                    if (error is! AssetNotFoundException) throw error;
                    var message =
                        "Could not find ${log.bold(executable + ".dart")}";
                    if (package != entrypoint.root.name) {
                      message += " in package ${log.bold(server.package)}";
                    }
                    log.error("$message.");
                    log.fine(new Chain.forTrace(stackTrace));
                    return exit_codes.NO_INPUT;
                  })));
                }
                if (package == entrypoint.root.name) {
                  environment.serveDirectory(rootDir).then((x1) {
                    try {
                      server = x1;
                      join3();
                    } catch (e1) {
                      completer0.completeError(e1);
                    }
                  }, onError: (e2) {
                    completer0.completeError(e2);
                  });
                } else {
                  var dep = entrypoint.root.immediateDependencies.firstWhere(
                      ((dep) => dep.name == package),
                      orElse: (() => null));
                  join4() {
                    environment.servePackageBinDirectory(package).then((x2) {
                      try {
                        server = x2;
                        join3();
                      } catch (e3) {
                        completer0.completeError(e3);
                      }
                    }, onError: (e4) {
                      completer0.completeError(e4);
                    });
                  }
                  if (dep == null) {
                    join5() {
                      join4();
                    }
                    if (environment.graph.packages.containsKey(package)) {
                      dataError(
                          'Package "${package}" is not an immediate dependency.\n'
                              'Cannot run executables in transitive dependencies.');
                      join5();
                    } else {
                      dataError(
                          'Could not find package "${package}". Did you forget to ' 'add a dependency?');
                      join5();
                    }
                  } else {
                    join4();
                  }
                }
              } catch (e0) {
                completer0.completeError(e0);
              }
            }, onError: (e5) {
              completer0.completeError(e5);
            });
          }
          if (parts.length > 1) {
            assert(!isGlobal && package == entrypoint.root.name);
            rootDir = parts.first;
            join2();
          } else {
            executable = p.join("bin", executable);
            join2();
          }
        }
        if (!isGlobal && fileExists(localSnapshotPath)) {
          completer0.complete(
              _runCachedExecutable(entrypoint, localSnapshotPath, args));
        } else {
          join1();
        }
      }
      if (log.verbosity == log.Verbosity.NORMAL) {
        log.verbosity = log.Verbosity.WARNING;
        join0();
      } else {
        join0();
      }
    } catch (e6) {
      completer0.completeError(e6);
    }
  });
  return completer0.future;
}
Future<int> runSnapshot(String path, Iterable<String> args, {recompile(),
    bool checked: false}) {
  final completer0 = new Completer();
  scheduleMicrotask(() {
    try {
      var vmArgs = [path]..addAll(args);
      join0() {
        var stdin1;
        var stdin2;
        join1() {
          runProcess(input) {
            final completer0 = new Completer();
            scheduleMicrotask(() {
              try {
                Process.start(Platform.executable, vmArgs).then((x0) {
                  try {
                    var process = x0;
                    process.stderr.listen(stderr.add);
                    process.stdout.listen(stdout.add);
                    input.listen(process.stdin.add);
                    completer0.complete(process.exitCode);
                  } catch (e0) {
                    completer0.completeError(e0);
                  }
                }, onError: (e1) {
                  completer0.completeError(e1);
                });
              } catch (e2) {
                completer0.completeError(e2);
              }
            });
            return completer0.future;
          }
          runProcess(stdin1).then((x0) {
            try {
              var exitCode = x0;
              join2() {
                recompile().then((x1) {
                  try {
                    x1;
                    completer0.complete(runProcess(stdin2));
                  } catch (e1) {
                    completer0.completeError(e1);
                  }
                }, onError: (e2) {
                  completer0.completeError(e2);
                });
              }
              if (recompile == null || exitCode != 255) {
                completer0.complete(exitCode);
              } else {
                join2();
              }
            } catch (e0) {
              completer0.completeError(e0);
            }
          }, onError: (e3) {
            completer0.completeError(e3);
          });
        }
        if (recompile == null) {
          stdin1 = stdin;
          join1();
        } else {
          var pair = tee(stdin);
          stdin1 = pair.first;
          stdin2 = pair.last;
          join1();
        }
      }
      if (checked) {
        vmArgs.insert(0, "--checked");
        join0();
      } else {
        join0();
      }
    } catch (e4) {
      completer0.completeError(e4);
    }
  });
  return completer0.future;
}
Future<int> _runCachedExecutable(Entrypoint entrypoint, String snapshotPath,
    List<String> args) {
  return runSnapshot(snapshotPath, args, checked: true, recompile: () {
    log.fine("Precompiled executable is out of date.");
    return entrypoint.precompileExecutables();
  });
}