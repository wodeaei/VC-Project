/////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk/settings.cpp
// Purpose:
// Author:      Robert Roebling
// Modified by: Mart Raudsepp (GetMetric)
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/settings.h"

#ifndef WX_PRECOMP
    #include "wx/cmndata.h"
    #include "wx/toplevel.h"
#endif

#include "wx/fontutil.h"
#include "wx/fontenum.h"

#include <gtk/gtk.h>
#include "wx/gtk/private/win_gtk.h"

bool wxGetFrameExtents(GdkWindow* window, int* left, int* right, int* top, int* bottom);

// ----------------------------------------------------------------------------
// wxSystemSettings implementation
// ----------------------------------------------------------------------------

static wxFont gs_fontSystem;

static GtkContainer* ContainerWidget()
{
    static GtkContainer* s_widget;
    if (s_widget == NULL)
    {
        s_widget = GTK_CONTAINER(gtk_fixed_new());
        GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(s_widget));
    }
    return s_widget;
}

extern "C" {
static void style_set(GtkWidget*, GtkStyle*, void*)
{
    gs_fontSystem = wxNullFont;
}
}

static const GtkStyle* ButtonStyle()
{
    static GtkWidget* s_widget;
    if (s_widget == NULL)
    {
        s_widget = gtk_button_new();
        gtk_container_add(ContainerWidget(), s_widget);
        gtk_widget_ensure_style(s_widget);
        g_signal_connect(s_widget, "style_set", G_CALLBACK(style_set), NULL);
    }
    return s_widget->style;
}

static const GtkStyle* ListStyle()
{
    static GtkWidget* s_widget;
    if (s_widget == NULL)
    {
        s_widget = gtk_tree_view_new_with_model(
            GTK_TREE_MODEL(gtk_list_store_new(1, G_TYPE_INT)));
        gtk_container_add(ContainerWidget(), s_widget);
        gtk_widget_ensure_style(s_widget);
    }
    return s_widget->style;
}

static const GtkStyle* TextCtrlStyle()
{
    static GtkWidget* s_widget;
    if (s_widget == NULL)
    {
        s_widget = gtk_text_view_new();
        gtk_container_add(ContainerWidget(), s_widget);
        gtk_widget_ensure_style(s_widget);
    }
    return s_widget->style;
}

static const GtkStyle* MenuItemStyle()
{
    static GtkWidget* s_widget;
    if (s_widget == NULL)
    {
        s_widget = gtk_menu_item_new();
        gtk_container_add(ContainerWidget(), s_widget);
        gtk_widget_ensure_style(s_widget);
    }
    return s_widget->style;
}

static const GtkStyle* MenuBarStyle()
{
    static GtkWidget* s_widget;
    if (s_widget == NULL)
    {
        s_widget = gtk_menu_bar_new();
        gtk_container_add(ContainerWidget(), s_widget);
        gtk_widget_ensure_style(s_widget);
    }
    return s_widget->style;
}

static const GtkStyle* ToolTipStyle()
{
    static GtkWidget* s_widget;
    if (s_widget == NULL)
    {
        s_widget = gtk_window_new(GTK_WINDOW_POPUP);
        const char* name = "gtk-tooltip";
        if (gtk_check_version(2, 11, 0))
            name = "gtk-tooltips";
        gtk_widget_set_name(s_widget, name);
        gtk_widget_ensure_style(s_widget);
    }
    return s_widget->style;
}

wxColour wxSystemSettingsNative::GetColour( wxSystemColour index )
{
    wxColor color;
    switch (index)
    {
        case wxSYS_COLOUR_SCROLLBAR:
        case wxSYS_COLOUR_BACKGROUND:
        //case wxSYS_COLOUR_DESKTOP:
        case wxSYS_COLOUR_INACTIVECAPTION:
        case wxSYS_COLOUR_MENU:
        case wxSYS_COLOUR_WINDOWFRAME:
        case wxSYS_COLOUR_ACTIVEBORDER:
        case wxSYS_COLOUR_INACTIVEBORDER:
        case wxSYS_COLOUR_BTNFACE:
        //case wxSYS_COLOUR_3DFACE:
        case wxSYS_COLOUR_3DLIGHT:
            color = wxColor(ButtonStyle()->bg[GTK_STATE_NORMAL]);
            break;

        case wxSYS_COLOUR_WINDOW:
            color = wxColor(TextCtrlStyle()->base[GTK_STATE_NORMAL]);
            break;

        case wxSYS_COLOUR_MENUBAR:
            color = wxColor(MenuBarStyle()->bg[GTK_STATE_NORMAL]);
            break;

        case wxSYS_COLOUR_3DDKSHADOW:
            color = *wxBLACK;
            break;

        case wxSYS_COLOUR_GRAYTEXT:
        case wxSYS_COLOUR_BTNSHADOW:
        //case wxSYS_COLOUR_3DSHADOW:
            {
                wxColour faceColour(GetColour(wxSYS_COLOUR_3DFACE));
                color =
                   wxColour((unsigned char) (faceColour.Red() * 2 / 3),
                            (unsigned char) (faceColour.Green() * 2 / 3),
                            (unsigned char) (faceColour.Blue() * 2 / 3));
            }
            break;

        case wxSYS_COLOUR_BTNHIGHLIGHT:
        //case wxSYS_COLOUR_BTNHILIGHT:
        //case wxSYS_COLOUR_3DHIGHLIGHT:
        //case wxSYS_COLOUR_3DHILIGHT:
            color = *wxWHITE;
            break;

        case wxSYS_COLOUR_HIGHLIGHT:
            color = wxColor(ButtonStyle()->bg[GTK_STATE_SELECTED]);
            break;

        case wxSYS_COLOUR_LISTBOX:
            color = wxColor(ListStyle()->base[GTK_STATE_NORMAL]);
            break;

        case wxSYS_COLOUR_LISTBOXTEXT:
            color = wxColor(ListStyle()->text[GTK_STATE_NORMAL]);
            break;

        case wxSYS_COLOUR_MENUTEXT:
        case wxSYS_COLOUR_WINDOWTEXT:
        case wxSYS_COLOUR_CAPTIONTEXT:
        case wxSYS_COLOUR_INACTIVECAPTIONTEXT:
        case wxSYS_COLOUR_BTNTEXT:
            color = wxColor(ButtonStyle()->fg[GTK_STATE_NORMAL]);
            break;

        case wxSYS_COLOUR_INFOBK:
            color = wxColor(ToolTipStyle()->bg[GTK_STATE_NORMAL]);
            break;

        case wxSYS_COLOUR_INFOTEXT:
            color = wxColor(ToolTipStyle()->fg[GTK_STATE_NORMAL]);
            break;

        case wxSYS_COLOUR_HIGHLIGHTTEXT:
            color = wxColor(ButtonStyle()->fg[GTK_STATE_SELECTED]);
            break;

        case wxSYS_COLOUR_APPWORKSPACE:
            color = *wxWHITE;    // ?
            break;

        case wxSYS_COLOUR_ACTIVECAPTION:
        case wxSYS_COLOUR_MENUHILIGHT:
            color = wxColor(MenuItemStyle()->bg[GTK_STATE_SELECTED]);
            break;

        case wxSYS_COLOUR_HOTLIGHT:
        case wxSYS_COLOUR_GRADIENTACTIVECAPTION:
        case wxSYS_COLOUR_GRADIENTINACTIVECAPTION:
            // TODO
            color = *wxBLACK;
            break;

        case wxSYS_COLOUR_MAX:
        default:
            wxFAIL_MSG( wxT("unknown system colour index") );
            color = *wxWHITE;
            break;
    }

    wxASSERT(color.IsOk());
    return color;
}

wxFont wxSystemSettingsNative::GetFont( wxSystemFont index )
{
    wxFont font;
    switch (index)
    {
        case wxSYS_OEM_FIXED_FONT:
        case wxSYS_ANSI_FIXED_FONT:
        case wxSYS_SYSTEM_FIXED_FONT:
            font = *wxNORMAL_FONT;
            break;

        case wxSYS_ANSI_VAR_FONT:
        case wxSYS_SYSTEM_FONT:
        case wxSYS_DEVICE_DEFAULT_FONT:
        case wxSYS_DEFAULT_GUI_FONT:
            if (!gs_fontSystem.Ok())
            {
                wxNativeFontInfo info;
                info.description = ButtonStyle()->font_desc;
                gs_fontSystem = wxFont(info);

#if wxUSE_FONTENUM
                // (try to) heal the default font (on some common systems e.g. Ubuntu
                // it's "Sans Serif" but the real font is called "Sans"):
                if (!wxFontEnumerator::IsValidFacename(gs_fontSystem.GetFaceName()) &&
                    gs_fontSystem.GetFaceName() == "Sans Serif")
                    gs_fontSystem.SetFaceName("Sans");
#endif // wxUSE_FONTENUM

                info.description = NULL;
            }
            font = gs_fontSystem;
            break;

        default:
            break;
    }

    wxASSERT( font.IsOk() );

    return font;
}

// helper: return the GtkSettings either for the screen the current window is
// on or for the default screen if window is NULL
static GtkSettings *GetSettingsForWindowScreen(GdkWindow *window)
{
    return window ? gtk_settings_get_for_screen(gdk_drawable_get_screen(window))
                  : gtk_settings_get_default();
}

static int GetBorderWidth(wxSystemMetric index, wxWindow* win)
{
    if (win->m_wxwindow)
    {
        wxPizza* pizza = WX_PIZZA(win->m_wxwindow);
        int x, y;
        pizza->get_border_widths(x, y);
        switch (index)
        {
            case wxSYS_BORDER_X:
            case wxSYS_EDGE_X:
            case wxSYS_FRAMESIZE_X:
                return x;
            default:
                return y;
        }
    }
    return -1;
}

int wxSystemSettingsNative::GetMetric( wxSystemMetric index, wxWindow* win )
{
    GdkWindow *window = NULL;
    if(win && GTK_WIDGET_REALIZED(win->GetHandle()))
        window = win->GetHandle()->window;

    switch (index)
    {
        case wxSYS_BORDER_X:
        case wxSYS_BORDER_Y:
        case wxSYS_EDGE_X:
        case wxSYS_EDGE_Y:
        case wxSYS_FRAMESIZE_X:
        case wxSYS_FRAMESIZE_Y:
            if (win)
            {
                wxTopLevelWindow *tlw = wxDynamicCast(win, wxTopLevelWindow);
                if (!tlw)
                    return GetBorderWidth(index, win);
                else if (window)
                {
                    // Get the frame extents from the windowmanager.
                    // In most cases the top extent is the titlebar, so we use the bottom extent
                    // for the heights.
                    int right, bottom;
                    if (wxGetFrameExtents(window, NULL, &right, NULL, &bottom))
                    {
                        switch (index)
                        {
                            case wxSYS_BORDER_X:
                            case wxSYS_EDGE_X:
                            case wxSYS_FRAMESIZE_X:
                                return right; // width of right extent
                            default:
                                return bottom; // height of bottom extent
                        }
                    }
                }
            }

            return -1; // no window specified

        case wxSYS_CURSOR_X:
        case wxSYS_CURSOR_Y:
                return gdk_display_get_default_cursor_size(
                            window ? gdk_drawable_get_display(window)
                                   : gdk_display_get_default());

        case wxSYS_DCLICK_X:
        case wxSYS_DCLICK_Y:
            gint dclick_distance;
            g_object_get(GetSettingsForWindowScreen(window),
                            "gtk-double-click-distance", &dclick_distance, NULL);

            return dclick_distance * 2;

        case wxSYS_DCLICK_MSEC:
            gint dclick;
            g_object_get(GetSettingsForWindowScreen(window),
                            "gtk-double-click-time", &dclick, NULL);
            return dclick;

        case wxSYS_DRAG_X:
        case wxSYS_DRAG_Y:
            gint drag_threshold;
            g_object_get(GetSettingsForWindowScreen(window),
                            "gtk-dnd-drag-threshold", &drag_threshold, NULL);

            // The correct thing here would be to double the value
            // since that is what the API wants. But the values
            // are much bigger under GNOME than under Windows and
            // just seem to much in many cases to be useful.
            // drag_threshold *= 2;

            return drag_threshold;

        case wxSYS_ICON_X:
        case wxSYS_ICON_Y:
            return 32;

        case wxSYS_SCREEN_X:
            if (window)
                return gdk_screen_get_width(gdk_drawable_get_screen(window));
            else
                return gdk_screen_width();

        case wxSYS_SCREEN_Y:
            if (window)
                return gdk_screen_get_height(gdk_drawable_get_screen(window));
            else
                return gdk_screen_height();

        case wxSYS_HSCROLL_Y:
        case wxSYS_VSCROLL_X:
            return 15;

        case wxSYS_CAPTION_Y:
            if (!window)
                // No realized window specified, and no implementation for that case yet.
                return -1;

            wxASSERT_MSG( wxDynamicCast(win, wxTopLevelWindow),
                          wxT("Asking for caption height of a non toplevel window") );

            // Get the height of the top windowmanager border.
            // This is the titlebar in most cases. The titlebar might be elsewhere, and
            // we could check which is the thickest wm border to decide on which side the
            // titlebar is, but this might lead to interesting behaviours in used code.
            // Reconsider when we have a way to report to the user on which side it is.
            {
                int top;
                if (wxGetFrameExtents(window, NULL, NULL, &top, NULL))
                {
                    return top; // top frame extent
                }
            }

            // Try a default approach without a window pointer, if possible
            // ...

            return -1;

        case wxSYS_PENWINDOWS_PRESENT:
            // No MS Windows for Pen computing extension available in X11 based gtk+.
            return 0;

        default:
            return -1;   // metric is unknown
    }
}

bool wxSystemSettingsNative::HasFeature(wxSystemFeature index)
{
    switch (index)
    {
        case wxSYS_CAN_ICONIZE_FRAME:
            return false;

        case wxSYS_CAN_DRAW_FRAME_DECORATIONS:
            return true;

        default:
            return false;
    }
}
