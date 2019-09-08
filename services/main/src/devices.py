import cherrypy
import os
import json
import datetime
import influxdb
import src.schemas as schema


class Devices(object):
    def __init__(self):
        pass

    exposed = True

    @cherrypy.tools.json_out()
    def GET(self, eventalias, intid, uuid):
        try:
            event = schema.Event.objects.get(alias=eventalias)
        except Exception as e:
            raise cherrypy.HTTPError(404, str(e))

        try:
            device = event.device_groups.get(
                intid=intid).devices.get(uuid=uuid)
            if device is not None:
                return {
                    "status": 200,
                    "data": json.loads(device.to_json())
                }

        except Exception as e:
            raise cherrypy.HTTPError(404, str(e))

        return {
            "status": 404,
            "message": "Device Not found"
        }

    @cherrypy.tools.json_in()
    @cherrypy.tools.json_out()
    def POST(self, eventalias, intid):
        try:
            event = schema.Event.objects.get(alias=eventalias)
        except Exception as e:
            raise cherrypy.HTTPError(404, str(e))

        device = schema.Device()
        device.controller_type = cherrypy.request.json.get('controller_type')

        notfound = True

        for idx, groupidx in enumerate(event.device_groups):
            if str(groupidx['intid']) == intid:
                event.device_groups[idx].devices.append(device)
                notfound = False
                break

        if notfound is True:
            raise cherrypy.HTTPError(
                404, 'Group not found')

        try:
            event.save()
        except Exception as e:
            raise cherrypy.HTTPError(400, str(e))

        cherrypy.response.status = 200
        return {
            "status": 200,
            "data": json.loads(event.to_json())
        }

    def DELETE(self, eventalias, intid, uuid):
        try:
            event = schema.Event.objects.get(alias=eventalias)
        except Exception as e:
            raise cherrypy.HTTPError(404, str(e))

        for idx, groupidx in enumerate(event.device_groups):
            if str(groupidx['intid']) == intid:
                for idy, deviceidx in enumerate(event.device_groups[idx].devices):
                    if str(deviceidx['uuid']) == uuid:
                        try:
                            event.device_groups[idx].devices.pop(idy)
                            event.save()
                            return {
                                "status": 200,
                                "data": "Deleted"
                            }
                        except Exception as e:
                            raise cherrypy.HTTPError(500, str(e))

        return {
            "status": 400,
            "message": "Couldn't detele Device"
        }
