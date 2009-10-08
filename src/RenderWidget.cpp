/*  Berkelium Implementation
 *  RenderWidget.cpp
 *
 *  Copyright (c) 2009, Patrick Reiter Horn
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of Sirikata nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "chrome/browser/renderer_host/backing_store.h"
#if defined(OS_LINUX)
#include "webkit/glue/plugins/gtk_plugin_container_manager.h"
#include "webkit/glue/plugins/gtk_plugin_container.h"
#include <gtk/gtk.h>
#include <gtk/gtkwindow.h>
#endif

#include "berkelium/Platform.hpp"
#include "RenderWidget.hpp"
#include "WindowImpl.hpp"

#include <iostream>

namespace Berkelium {

RenderWidget::RenderWidget(WindowImpl *winImpl, RenderWidgetHost* host) {
    mHost = host;
    mFocused = true;
    mBacking = NULL;
    mWindow = winImpl;
}

RenderWidget::~RenderWidget() {
    if (mBacking) {
        delete mBacking;
    }
}

void RenderWidget::InitAsPopup(RenderWidgetHostView* parent_host_view,
                           const gfx::Rect& pos){
    SetSize(pos.size());
}

  // Returns the associated RenderWidgetHost.
RenderWidgetHost* RenderWidget::GetRenderWidgetHost() const{
    return mHost;
}

  // Notifies the View that it has become visible.
void RenderWidget::DidBecomeSelected(){
}

  // Notifies the View that it has been hidden.
void RenderWidget::WasHidden(){
}

  // Tells the View to size itself to the specified size.
void RenderWidget::SetSize(const gfx::Size& size){
    mRect.set_width(size.width());
    mRect.set_height(size.height());
}

  // Retrieves the native view used to contain plugins and identify the
  // renderer in IPC messages.
gfx::NativeView RenderWidget::GetNativeView(){
    return gfx::NativeView();
}

  // Actually set/take focus to/from the associated View component.
void RenderWidget::Focus(){
    mFocused = true;
}
void RenderWidget::Blur(){
    mFocused = false;
}

  // Returns true if the View currently has the focus.
bool RenderWidget::HasFocus(){
    return mFocused;
}

  // Shows/hides the view.  These must always be called together in pairs.
  // It is not legal to call Hide() multiple times in a row.
void RenderWidget::Show(){
}
void RenderWidget::Hide(){
}

  // Retrieve the bounds of the View, in screen coordinates.
gfx::Rect RenderWidget::GetViewBounds() const{
    return mRect;
}

  // Sets the cursor to the one associated with the specified cursor_type
void RenderWidget::UpdateCursor(const WebCursor& cursor){
}

  // Indicates whether the page has finished loading.
void RenderWidget::SetIsLoading(bool is_loading){
}

  // Enable or disable IME for the view.
void RenderWidget::IMEUpdateStatus(int control, const gfx::Rect& caret_rect){
}

  // Informs the view that a portion of the widget's backing store was painted.
  // The view should ensure this gets copied to the screen.
  //
  // There are subtle performance implications here.  The RenderWidget gets sent
  // a paint ack after this returns, so if the view only ever invalidates in
  // response to this, then on Windows, where WM_PAINT has lower priority than
  // events which can cause renderer resizes/paint rect updates, e.g.
  // drag-resizing can starve painting; this function thus provides the view its
  // main chance to ensure it stays painted and not just invalidated.  On the
  // other hand, if this always blindly paints, then if we're already in the
  // midst of a paint on the callstack, we can double-paint unnecessarily.
  // (Worse, we might recursively call RenderWidgetHost::GetBackingStore().)
  // Thus implementers should generally paint as much of |rect| as possible
  // synchronously with as little overpainting as possible.
void RenderWidget::DidPaintRect(const gfx::Rect& rect){
//    std::cout << "Painted rect "<<rect << std::endl;
}

  // Informs the view that a portion of the widget's backing store was scrolled
  // by dx pixels horizontally and dy pixels vertically. The view should copy
  // the exposed pixels from the backing store of the render widget (which has
  // already been scrolled) onto the screen.
void RenderWidget::DidScrollRect(
      const gfx::Rect& rect, int dx, int dy){
}

  // Notifies the View that the renderer has ceased to exist.
void RenderWidget::RenderViewGone(){
}

  // Tells the View to destroy itself.
void RenderWidget::Destroy(){
    // FIXME: Unimplemented.
    delete this;
}

  // Tells the View that the tooltip text for the current mouse position over
  // the page has changed.
void RenderWidget::SetTooltipText(const std::wstring& tooltip_text){
}

  // Notifies the View that the renderer text selection has changed.
void RenderWidget::SelectionChanged(const std::string& text) {
}

  // Tells the View whether the context menu is showing. This is used on Linux
  // to suppress updates to webkit focus for the duration of the show.
void RenderWidget::ShowingContextMenu(bool showing) {
}

  // Allocate a backing store for this view
BackingStore* RenderWidget::AllocBackingStore(const gfx::Size& size) {
    SetSize(size);

    if (!mBacking) {
        mBacking = new BackingStore(mHost, size); // will ignore paints on linux!
    }
    return mBacking;
}

#if defined(OS_MACOSX)
  // Display a native control popup menu for WebKit.
void RenderWidget::ShowPopupWithItems(gfx::Rect bounds,
                                  int item_height,
                                  int selected_item,
                                  const std::vector<WebMenuItem>& items){
}

  // Get the view's position on the screen.
gfx::Rect RenderWidget::GetWindowRect(){
    return gfx::Rect(0, 0, mRect.width(), mRect.height());
}

  // Get the view's window's position on the screen.
gfx::Rect RenderWidget::GetRootWindowRect(){
    return mRect;
}

  // Set the view's active state (i.e., tint state of controls).
void RenderWidget::SetActive(bool active){
}
#endif

#if defined(OS_LINUX)

static std::map<gfx::PluginWindowHandle, GtkWidget*> activeWidgets;

void RealizeCallback(GtkWidget* widget, void* user_data) {
    gfx::PluginWindowHandle id = (gfx::PluginWindowHandle)user_data;
    if (id)
        gtk_socket_add_id(GTK_SOCKET(widget), id);
}


void RenderWidget::CreatePluginContainer(gfx::PluginWindowHandle id){
    std::cerr<<"CREATED PLUGIN CONTAINER: "<<id<<std::endl;
    assert(activeWidgets.find(id) == activeWidgets.end());

    if (!gdk_display_get_default()) {
        const char *argv0 = "gtk_program";
        const char **argv = &argv0;
        int argc = 1;
        gtk_init(&argc, const_cast<char***>(&argv));
    }

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    activeWidgets.insert(
        std::pair<gfx::PluginWindowHandle, GtkWidget*>(id, window));

    const GURL &current = mWindow->getCurrentURL();
    std::string pluginTitle = "[Plugin from "+current.spec()+"]";
    gtk_window_set_title (GTK_WINDOW(window), pluginTitle.c_str());

    GtkWidget *widget = gtk_plugin_container_new();

    // The Realize callback is responsible for adding the plug into the socket.
    // The reason is 2-fold:
    // - the plug can't be added until the socket is realized, but this may not
    // happen until the socket is attached to a top-level window, which isn't the
    // case for background tabs.
    // - when dragging tabs, the socket gets unrealized, which breaks the XEMBED
    // connection. We need to make it again when the tab is reattached, and the
    // socket gets realized again.
    //
    // Note, the RealizeCallback relies on the plugin_window_to_widget_map_ to
    // have the mapping.
    g_signal_connect(G_OBJECT(widget), "realize",
                     G_CALLBACK(RealizeCallback), (void*)id);

    // Don't destroy the widget when the plug is removed.
    g_signal_connect(G_OBJECT(widget), "plug-removed",
                     G_CALLBACK(gtk_true), NULL);

    gtk_container_add(GTK_CONTAINER(window), widget);
    gtk_widget_show(widget);

}
void RenderWidget::DestroyPluginContainer(gfx::PluginWindowHandle id){
    std::cerr<<"DESTROYED PLUGIN CONTAINER: "<<id<<std::endl;
    std::map<gfx::PluginWindowHandle, GtkWidget*>::iterator iter;
    iter = activeWidgets.find(id);
    if (iter != activeWidgets.end()) {
        GtkWidget * w = iter->second;
        gtk_widget_destroy(w);
        activeWidgets.erase(iter);
    }
}
#endif

  // Moves all plugin windows as described in the given list.
void RenderWidget::MovePluginWindows(
    const std::vector<webkit_glue::WebPluginGeometry>& moves)
{
#ifdef OS_LINUX
    for (size_t i = 0; i < moves.size(); i++) {
        std::map<gfx::PluginWindowHandle, GtkWidget*>::iterator iter;
        iter = activeWidgets.find(moves[i].window);
        if (iter != activeWidgets.end()) {
            gtk_window_resize(
                GTK_WINDOW (iter->second),
                moves[i].window_rect.width(),
                moves[i].window_rect.height());
            if (moves[i].visible) {
                gtk_widget_show (iter->second);
            } else {
                gtk_widget_hide (iter->second);
            }
        }
    }
#endif
}



}


