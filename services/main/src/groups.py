import cherrypy
import os
import json
import datetime
import influxdb
import src.schemas as schema


class Groups(object):
    def __init__(self, client):
        self.client = client

    exposed = True

    @cherrypy.tools.json_out()
    def GET(self, eventalias, intid):
        try:
            event = schema.Event.objects.get(alias=eventalias)
        except Exception as e:
            raise cherrypy.HTTPError(404, str(e))

        try:
            group = event.device_groups.get(intid=intid)
            if group is not None:
                return {
                    "status": 200,
                    "data": json.loads(group.to_json())
                }

        except Exception as e:
            raise cherrypy.HTTPError(404, str(e))

        return {
            "status": 404,
            "message": "Group Not found"
        }

    @cherrypy.tools.json_in()
    @cherrypy.tools.json_out()
    def POST(self, eventalias):
        try:
            event = schema.Event.objects.get(alias=eventalias)
        except Exception as e:
            raise cherrypy.HTTPError(404, str(e))

        group = schema.Group()
        group.intid = cherrypy.request.json.get('intid')
        group.description = cherrypy.request.json.get('description')

        event.device_groups.append(group)

        try:
            event.save()
        except Exception as e:
            raise cherrypy.HTTPError(400, str(e))

        cherrypy.response.status = 200
        return {
            "status": 200,
            "data": json.loads(event.to_json())
        }

    def DELETE(self, eventalias, intid):
        try:
            event = schema.Event.objects.get(alias=eventalias)
        except Exception as e:
            raise cherrypy.HTTPError(404, str(e))

        group = event.device_groups.get(intid=intid)

        if group is not None:
            try:
                event.device_groups.remove(group)
                event.save()
            except Exception as e:
                raise cherrypy.HTTPError(500, str(e))

            return {
                "status": 200,
                "data": "Deleted"
            }

        return {
            "status": 400,
            "message": "Couldn't detele Group"
        }
