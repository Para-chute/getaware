import cherrypy
import os
import json
import datetime
import influxdb
import src.schemas as schema


class Events(object):
    def __init__(self, client):
        self.client = client

    exposed = True

    @cherrypy.tools.json_out()
    def GET(self, alias):
        try:
            event = schema.Event.objects.get(alias=alias)
        except Exception as e:
            raise cherrypy.HTTPError(404, str(e))

        return {
            "status": 200,
            "data": json.loads(event.to_json())
        }

    @cherrypy.tools.json_in()
    @cherrypy.tools.json_out()
    def POST(self):
        event = schema.Event()
        event.name = cherrypy.request.json.get('name')
        event.alias = cherrypy.request.json.get(
            'name').replace(" ", "").lower()

        try:
            event.save()
        except Exception as e:
            raise cherrypy.HTTPError(400, str(e))

        self.client.create_database(event.alias)

        cherrypy.response.status = 200
        return {
            "status": 200,
            "data": json.loads(event.to_json())
        }

    def DELETE(self, alias):
        try:
            event = schema.Event.objects.get(alias=alias)
        except Exception as e:
            raise cherrypy.HTTPError(404, str(e))

        try:
            event.delete()
            self.client.drop_database(event.alias)
        except Exception as e:
            raise cherrypy.HTTPError(500, str(e))

        return {
            "status": 200,
            "message": f"Event {alias} deteled successfully"
        }
