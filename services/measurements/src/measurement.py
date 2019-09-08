import cherrypy
import os
import json
import datetime
from dateutil.parser import parse
import influxdb
import requests


class Measurement(object):
    def __init__(self, client):
        self.client = client

    exposed = True

    @cherrypy.tools.json_out()
    def GET(self, eventalias, uuid, startdate=None, enddate=datetime.datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%SZ')):

        if startdate is None:
            try:
                response = requests.get(f"http://main:5000/api/v1/events",
                                        params={
                                            "alias": eventalias
                                        })

                if response.status_code is 200:
                    content = json.loads(response.text)
                    date = parse(content["data"]["created_at"])
                    startdate = date.strftime('%Y-%m-%dT%H:%M:%SZ')
            except Exception as e:
                raise cherrypy.HTTPError(400, str(e))

        self.client.switch_database(eventalias)
        query = f'select * from "{uuid}"'
        #query = f"""SELECT * FROM "{uuid}" WHERE time >= '{startdate}' and time <= '{enddate}'"""
        result = self.client.query(query).raw

        return {
            "status": 200,
            "data": result
        }

    @cherrypy.tools.json_in()
    @cherrypy.tools.json_out()
    def POST(self):
        self.client.switch_database(cherrypy.request.json.get('eventalias'))
        influxdb_json = {
            "measurement": cherrypy.request.json.get('uuid'),
            "fields": {
                "value": cherrypy.request.json.get('value')
            }
        }
        r = self.client.write_points([influxdb_json])
        print(influxdb_json)

        if r is True:
            cherrypy.response.status = 200
            return {
                "status": 200
            }

        cherrypy.response.status = 400
        return {
            "status": 400,
        }
