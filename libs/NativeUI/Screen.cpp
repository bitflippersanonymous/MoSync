/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file Screen.cpp
 * @author Mikael Kindborg
 *
 * Class that represents a visible screen. Only one screen
 * is visible at a time.
 */

#include "Screen.h"
#include "ScreenListener.h"

namespace NativeUI
{

	/**
	 * Constructor.
	 */
	Screen::Screen() :
		Widget(MAW_SCREEN)
	{
	}

	/**
	 * Protected because only subclasses should use this constructor.
	 * @widgetType The string constant that identifies the widget type
	 * (one of the MAW_ constants).
	 */
	Screen::Screen(const MAUtil::String& widgetType) :
		Widget(widgetType)
	{
	}

	/**
	 * Destructor.
	 */
	Screen::~Screen()
	{
		mScreenListeners.clear();
	}

	/**
	 * Set the title of the screen.
	 * The title is used by tab screen to display a text on the tab indicator.
	 * @param title The screen title.
	 */
	void Screen::setTitle(const MAUtil::String& title)
	{
		setProperty(MAW_SCREEN_TITLE, title.c_str());
	}

	/**
	 * Set the icon of the screen. The icon is displayed
	 * on tab screens.
	 * @param imageHandle Handle to an image with the icon.
	 * @return The result code.
	 */
	int Screen::setIcon(const MAHandle imageHandle)
	{
		return setPropertyInt(MAW_SCREEN_ICON, imageHandle);
	}

    /**
     * Set the main widget of the screen.
     * Note: A screen can only have one main widget.
     * Use a layout as main widget and add child
     * widgets to the layout.
     * @param widget The widget to be set as main widget.
     * The ownership of the widget is passed to this function.
     * When the screen will be destroyed, the widget and it's child widgets
     * will be deleted.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the child could be added to the parent.
     * - #MAW_RES_INVALID_LAYOUT if the widget was added to a non-layout.
     * - #MAW_RES_ERROR if it could not be added for some other reason.
     */
	int Screen::setMainWidget(Widget* widget)
	{
	    int nrChildren = this->countChildWidgets();
	    for (int i = 0; i < nrChildren; i++)
	    {
	        this->removeChild(this->getChild(i));
	    }

	    return addChild(widget);
	}

	/**
	 * Show a screen. Only one screen at a time is visible.
	 * The previous screen will be hidden when showing a screen.
	 * Note: This method is only applicable to screens.
	 */
	void Screen::show()
	{
		maWidgetScreenShow(getWidgetHandle());
	}

	/**
	 * Called just before the screen begins rotating.
	 * Subclasses may override this method to perform additional actions
	 * immediately prior to the rotation.
	 */
	void Screen::orientationWillChange()
	{
		// No implementation required.
	}

	/**
	 * Called after the screen has finished rotating.
	 * Subclasses may override this method to perform additional actions
	 * after the rotation.
	 */
	 void Screen::orientationDidChange()
	 {
		// No implementation required
	 }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void Screen::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        if (widgetEventData->eventType == MAW_EVENT_SCREEN_ORIENTATION_WILL_CHANGE)
        {
            this->orientationWillChange();
        }
		else if (widgetEventData->eventType == MAW_EVENT_SCREEN_ORIENTATION_DID_CHANGE)
		{
			this->orientationDidChange();
		}
		else if ( MAW_EVENT_OPTIONS_MENU_ITEM_SELECTED == widgetEventData->eventType )
		{
			for (int i=0; i < mScreenListeners.size(); i++)
			{
				mScreenListeners[i]->optionsMenuItemSelected(
						this, widgetEventData->optionsMenuItem);
			}
		}
		else if( MAW_EVENT_OPTIONS_MENU_CLOSED == widgetEventData->eventType )
		{
			for (int i=0; i < mScreenListeners.size(); i++)
			{
				mScreenListeners[i]->optionsMenuClosed(this);
			}
		}
    }

	/**
	 * Add a new menu item to the Options Menu associated to this screen.
	 * Option Menus are Android specific concept, so this function is
	 * available only on this platform. The Options Menu is launched by
	 * pressing the Menu key. The options menu is where you should include
	 * actions and other options that are relevant to the current activity
	 * context, such as "Search," "Compose email," or "Settings".
	 * When opened, the first visible portion is the icon menu, which holds
	 * up to six menu items. If your menu includes more than six items, Android
	 * places the sixth item and the rest into the overflow menu, which the user
	 * can open by selecting More. Those items do not display icons.
	 * @param title The title associated for the new item. Can be left null.
	 * @param iconId The resource ID of the icon which will be used to lazily
	 * get the Drawable when this item is being shown. Can be set to -1 if
	 * no icon needed.
	 * @param iconPredefined Specifies if the icon is a project resource, or one
	 * of the predefined Android icons. It is false by default.
	 * @return The index on which the menu item was added in the options menu,
	 * an error code otherwise.
	 */
	int Screen::addOptionsMenuItem(const MAUtil::String title, int iconId, bool iconPredefined)
	{
		return maWidgetScreenAddOptionsMenuItem(
				getWidgetHandle(), title.c_str(), iconId, (iconPredefined ? 1 : 0) );
	}

	/**
	 * Remove the options menu from this screen.
	 * @return True if success, false otherwise ( for instance the options
	 * menu has no content.
	 */
	void Screen::removeOptionsMenu()
	{
		setProperty(MAW_SCREEN_REMOVE_OPTIONS_MENU,"");
	}

    /**
     * Add a screen event listener.
     * @param listener The listener that will receive screen events.
     */
    void Screen::addScreenListener(ScreenListener* listener)
    {
		addListenerToVector(mScreenListeners, listener);
    }

    /**
     * Remove the screen event listener.
     * @param listener The listener that receives screen events.
     */
    void Screen::removeScreenListener(ScreenListener* listener)
    {
		removeListenerFromVector(mScreenListeners, listener);
    }

} // namespace NativeUI
