import cherrypy
from mongoengine import connect
import json
import os

from src.measurement import Measurement
from influxdb import InfluxDBClient

# connect("measurements", host="mongodb://" + os.environ.get('DATABASE_USERNAME') +
#        ":" + os.environ.get('DATABASE_PASSWORD') +
#        "@db:" + str(27017) + '/?authSource=admin')

client = InfluxDBClient(host='influxdb', port=8086)
# client.create_database('water1')
# client.create_database('water2')


def json_error(status, message, traceback, version):
    cherrypy.response.headers['Content-Type'] = 'application/json'
    return json.dumps({'status': status, 'message': message})


if __name__ == '__main__':
    conf = {
        '/': {
            'request.dispatch': cherrypy.dispatch.MethodDispatcher(),
            'tools.response_headers.on': True,
            'tools.gzip.on': True,
            'error_page.default': json_error
        }
    }

    cherrypy.config.update({
        'server.socket_host': '0.0.0.0',
        'server.socket_port': 5000
    })

    cherrypy.tree.mount(Measurement(client), '/api/v1/measurements', conf)
    cherrypy.engine.start()
    cherrypy.engine.block()
