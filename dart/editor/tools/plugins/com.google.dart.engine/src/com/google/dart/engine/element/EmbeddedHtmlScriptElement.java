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
package com.google.dart.engine.element;

/**
 * The interface {@code EmbeddedHtmlScriptElement} defines the behavior of elements representing a
 * script tag in an HTML file having content that defines a Dart library.
 * 
 * @coverage dart.engine.element
 */
public interface EmbeddedHtmlScriptElement extends HtmlScriptElement {

  /**
   * Return the library element defined by the content of the script tag, or {@code null} if the
   * content does not define a library.
   * 
   * @return the library element
   */
  public LibraryElement getScriptLibrary();
}
