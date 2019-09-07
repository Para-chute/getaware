import cherrypy
import os
import json
import datetime
from dateutil.parser import parse
#import src.schemas as schema
#from mongoengine import Q
import influxdb


class Measurement(object):
    def __init__(self, client):
        self.client = client

    exposed = True

    @cherrypy.tools.json_out()
    def GET(self, eventalias, uuid):
        self.client.switch_database(eventalias)
        query = f'select * from "{uuid}"'
        print(query)
        result = self.client.query(query).raw

        return {
            "status": 200,
            "data": result
        }

    @cherrypy.tools.json_in()
    @cherrypy.tools.json_out()
    def POST(self):
        self.client.switch_database(cherrypy.request.json.get('eventalias'))
        r = self.client.write_points([{
            "measurement": cherrypy.request.json.get('uuid'),
            "fields": {
                "value": cherrypy.request.json.get('value')
            }
        }])

        if r is True:
            cherrypy.response.status = 200
            return {
                "status": 200
            }

        cherrypy.response.status = 400
        return {
            "status": 400,
        }
