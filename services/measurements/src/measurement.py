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
    def GET(self, **params):
        # p_startdate = parse(params['startdate'])
        # p_enddate = parse(params['enddate'])

        # print(p_startdate)
        # print(p_enddate)
        # print(params['device_id'])

       # try:
       #     pass
        #            measurements = schema.Measurement.objects(
     #               Q(time__gte=p_startdate) & Q(time__lte=p_enddate) & Q(device_id=device_id)).all()
        # except Exception as e:
        #   raise cherrypy.HTTPError(400, str(e))
        self.client.switch_database('water2')
        self.client.get_list_measurements()
        result = self.client.query('select * from "66"')

        return {
            "status": 200,
            "data": result.raw
        }

    @cherrypy.tools.json_in()
    @cherrypy.tools.json_out()
    def POST(self):

        data = [
            {
                "measurement": cherrypy.request.json.get('deviceId'),
                "fields": {
                    "value": 0.64
                }
            }
        ]

        # measurement = schema.Measurement()
        # measurement.device_id = cherrypy.request.json.get('deviceId')
        # measurement.value = cherrypy.request.json.get('value')

        # self.client.write_points(json)
        self.client.ping()
        self.client.switch_database('water2')
        a = self.client.write_points(data)

        cherrypy.response.status = 200
        return {
            "status": 200,
            "measurement": a
        }
