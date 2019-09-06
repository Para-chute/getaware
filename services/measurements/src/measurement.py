import cherrypy
import os
import src.schemas as schema


class Measurement(object):
    def __init__(self):
        pass

    exposed = True

    @cherrypy.tools.json_in()
    @cherrypy.tools.json_out()
    def POST(self, **params):

        m = schema.Measurement()
        m.device_id = cherrypy.request.json.get('deviceId')
        m.value = cherrypy.request.json.get('value')

        try:
            m.save()
        except Exception as e:
            raise cherrypy.HTTPError(400, str(e))

        cherrypy.response.status = 200
        return {
            "status": 200,
            "message": "Posted"
        }
