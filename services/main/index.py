import cherrypy
from mongoengine import connect
import json
import os

from src.events import Events
from src.groups import Groups
from src.devices import Devices
from influxdb import InfluxDBClient


connect("main", host="mongodb://" + os.environ.get('DATABASE_USERNAME') +
        ":" + os.environ.get('DATABASE_PASSWORD') +
        "@db:" + str(27017) + '/?authSource=admin')

client = InfluxDBClient(host='influxdb', port=8086)


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

    cherrypy.tree.mount(Events(client), '/api/v1/events', conf)
    cherrypy.tree.mount(Groups(), '/api/v1/groups', conf)
    cherrypy.tree.mount(Devices(), '/api/v1/devices', conf)
    cherrypy.engine.start()
    cherrypy.engine.block()
