/*
 * Copyright (c) 2013, the Dart project authors.
 * 
 * Licensed under the Eclipse Public License v1.0 (the "License"); you may not use this file except
 * in compliance with the License. You may obtain a copy of the License at
 * 
 * http://www.eclipse.org/legal/epl-v10.html
 * 
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 */
package com.google.dart.tools.debug.ui.launch;

import com.google.dart.tools.debug.ui.internal.DartDebugUIPlugin;
import com.google.dart.tools.debug.ui.internal.DebugErrorHandler;
import com.google.dart.tools.debug.ui.internal.util.LaunchUtils;
import com.google.dart.tools.ui.instrumentation.UIInstrumentationBuilder;

import org.eclipse.core.resources.IResource;
import org.eclipse.debug.ui.ILaunchShortcut;
import org.eclipse.jface.action.IAction;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.StructuredSelection;
import org.eclipse.ui.IWorkbenchWindow;

/**
 * Action to run application in the VM
 */
public class RunInServerAction extends DartRunAbstractAction {

  public RunInServerAction() {
    this(null);
  }

  public RunInServerAction(IWorkbenchWindow window) {
    super(window, "Run", IAction.AS_PUSH_BUTTON);

    setActionDefinitionId("com.google.dart.tools.debug.ui.run.server");
    setImageDescriptor(DartDebugUIPlugin.getImageDescriptor("obj16/run_exc.png"));
  }

  @Override
  protected void doLaunch(UIInstrumentationBuilder instrumentation) {
    IResource resource = LaunchUtils.getSelectedResource(window);

    try {
      if (resource != null) {
        instrumentation.metric("Resource-Class", resource.getClass().toString());
        instrumentation.data("Resource-Name", resource.getName());

        // new launch config
        ILaunchShortcut shortcut = LaunchUtils.getServerLaunchShortcut();
        ISelection selection = new StructuredSelection(resource);
        launch(shortcut, selection, instrumentation);
      }
    } catch (Exception exception) {
      instrumentation.metric("Problem", "Exception launching " + exception.getClass().toString());
      instrumentation.data("Problem", "Exception launching " + exception.toString());

      DebugErrorHandler.errorDialog(window.getShell(), "Error Launching", "Unable to launch "
          + resource.getName() + ".", exception);
    }
  }
}
