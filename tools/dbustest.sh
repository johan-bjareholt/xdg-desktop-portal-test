#!/bin/sh
set -e

busctl --user tree com.bjareholt.johan.PortalTest
busctl --user introspect com.bjareholt.johan.PortalTest /com/bjareholt/johan/PortalTest
busctl --user -j call com.bjareholt.johan.PortalTest /com/bjareholt/johan/PortalTest org.freedesktop.portal.WindowActivity GetWindows
