from mongoengine import Document, StringField, ListField, DateTimeField, BooleanField, EmbeddedDocument, EmbeddedDocumentField, EmbeddedDocumentListField, UUIDField, ReferenceField, IntField, DecimalField, ObjectIdField
import datetime
import uuid


class Measurement(Document):
    device_id = ObjectIdField()
    time = DateTimeField(default=datetime.datetime.utcnow)
    value = DecimalField(
        required=True, precision=2, rounding='ROUND_HALF_UP')


class Device(Document):
    online = BooleanField(default=False)
    controller_type = StringField()
    created_at = DateTimeField(default=datetime.datetime.utcnow)
    last_update = DateTimeField(default=datetime.datetime.utcnow)


class Statistics(EmbeddedDocument):
    last_updated = DateTimeField(default=datetime.datetime.utcnow)
    total = DecimalField(default=0)
    mean_hour = DecimalField(default=0)
    mean_usage = DecimalField(default=0)


class Group(EmbeddedDocument):
    intid = IntField()
    description = StringField(required=True)
    statistics = EmbeddedDocumentField('Statistics')
    devices = ListField(ReferenceField('Device'))


class Event(Document):
    name = StringField(required=True)
    created_at = DateTimeField(default=datetime.datetime.utcnow)
    updated_at = DateTimeField(default=datetime.datetime.utcnow)
    begin_date = DateTimeField()
    end_date = DateTimeField()
    statistics_total = EmbeddedDocumentField('Statistics')
    device_groups = EmbeddedDocumentListField('Group')
