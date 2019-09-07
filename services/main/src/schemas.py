import mongoengine as db
import mongoengine_goodjson as gj
import datetime
import uuid


class Measurement(gj.Document):
    device_id = db.ObjectIdField()
    time = db.DateTimeField(default=datetime.datetime.utcnow)
    value = db.DecimalField(
        required=True, precision=2, rounding='ROUND_HALF_UP')


class Device(gj.Document):
    online = db.BooleanField(default=False)
    controller_type = db.StringField()
    created_at = db.DateTimeField(default=datetime.datetime.utcnow)
    last_update = db.DateTimeField(default=datetime.datetime.utcnow)


class Group(gj.EmbeddedDocument):
    intid = db.IntField(required=True, unique=True)
    description = db.StringField(required=True)
    devices = db.ListField(db.ReferenceField('Device'))


class Event(gj.Document):
    name = db.StringField(required=True, unique=True)
    alias = db.StringField(required=True, unique=True)
    created_at = db.DateTimeField(default=datetime.datetime.utcnow)
    updated_at = db.DateTimeField(default=datetime.datetime.utcnow)
    begin_date = db.DateTimeField()
    end_date = db.DateTimeField()
    device_groups = db.EmbeddedDocumentListField('Group')
