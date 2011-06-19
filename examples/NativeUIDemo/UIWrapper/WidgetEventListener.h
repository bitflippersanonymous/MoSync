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
 * @file WidgetEventListener.h
 * @author Mikael Kindborg
 *
 * Interface for widget event listeners.
 *
 * TODO: Add description of how to use.
 */

#ifndef MOSYNC_UI_WIDGETEVENTLISTENER_H_
#define MOSYNC_UI_WIDGETEVENTLISTENER_H_

#include <ma.h>
#include <MAUtil/String.h>
#include <MAUtil/Map.h>
#include <MAUtil/Environment.h>
#include <IX_WIDGET.h>

namespace MoSync
{
	namespace UI
	{

	// Forward declaration.
	class Widget;

	/**
	 * Interface for widget event listeners.
	 */
	class WidgetEventListener
	{
	public:
		/**
		 * This method is called when there is an event for this widget.
		 * @param widget The widget object of the event.
		 * @param widgetEventData The low-level event data.
		 */
		virtual void handleWidgetEvent(
			Widget* widget,
			MAWidgetEventData* widgetEventData) = 0;
	};

	} // namespace UI
} // namespace MoSync

#endif