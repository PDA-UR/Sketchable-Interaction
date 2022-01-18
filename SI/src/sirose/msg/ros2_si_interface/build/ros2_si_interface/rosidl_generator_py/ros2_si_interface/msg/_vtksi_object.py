# generated from rosidl_generator_py/resource/_idl.py.em
# with input from ros2_si_interface:msg/VTKSIObject.idl
# generated code does not contain a copyright notice


# Import statements for member types

# Member 'geometry'
import array  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_VTKSIObject(type):
    """Metaclass of message 'VTKSIObject'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('ros2_si_interface')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'ros2_si_interface.msg.VTKSIObject')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__vtksi_object
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__vtksi_object
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__vtksi_object
            cls._TYPE_SUPPORT = module.type_support_msg__msg__vtksi_object
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__vtksi_object

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class VTKSIObject(metaclass=Metaclass_VTKSIObject):
    """Message class 'VTKSIObject'."""

    __slots__ = [
        '_id',
        '_plugin',
        '_geometry',
        '_click',
        '_drag',
        '_x',
        '_y',
        '_alive',
        '_touch',
    ]

    _fields_and_field_types = {
        'id': 'int64',
        'plugin': 'string',
        'geometry': 'sequence<float>',
        'click': 'boolean',
        'drag': 'boolean',
        'x': 'float',
        'y': 'float',
        'alive': 'boolean',
        'touch': 'boolean',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('int64'),  # noqa: E501
        rosidl_parser.definition.UnboundedString(),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('float')),  # noqa: E501
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.id = kwargs.get('id', int())
        self.plugin = kwargs.get('plugin', str())
        self.geometry = array.array('f', kwargs.get('geometry', []))
        self.click = kwargs.get('click', bool())
        self.drag = kwargs.get('drag', bool())
        self.x = kwargs.get('x', float())
        self.y = kwargs.get('y', float())
        self.alive = kwargs.get('alive', bool())
        self.touch = kwargs.get('touch', bool())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.id != other.id:
            return False
        if self.plugin != other.plugin:
            return False
        if self.geometry != other.geometry:
            return False
        if self.click != other.click:
            return False
        if self.drag != other.drag:
            return False
        if self.x != other.x:
            return False
        if self.y != other.y:
            return False
        if self.alive != other.alive:
            return False
        if self.touch != other.touch:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @property  # noqa: A003
    def id(self):  # noqa: A003
        """Message field 'id'."""
        return self._id

    @id.setter  # noqa: A003
    def id(self, value):  # noqa: A003
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'id' field must be of type 'int'"
            assert value >= -9223372036854775808 and value < 9223372036854775808, \
                "The 'id' field must be an integer in [-9223372036854775808, 9223372036854775807]"
        self._id = value

    @property
    def plugin(self):
        """Message field 'plugin'."""
        return self._plugin

    @plugin.setter
    def plugin(self, value):
        if __debug__:
            assert \
                isinstance(value, str), \
                "The 'plugin' field must be of type 'str'"
        self._plugin = value

    @property
    def geometry(self):
        """Message field 'geometry'."""
        return self._geometry

    @geometry.setter
    def geometry(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'f', \
                "The 'geometry' array.array() must have the type code of 'f'"
            self._geometry = value
            return
        if __debug__:
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, float) for v in value) and
                 True), \
                "The 'geometry' field must be a set or sequence and each value of type 'float'"
        self._geometry = array.array('f', value)

    @property
    def click(self):
        """Message field 'click'."""
        return self._click

    @click.setter
    def click(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'click' field must be of type 'bool'"
        self._click = value

    @property
    def drag(self):
        """Message field 'drag'."""
        return self._drag

    @drag.setter
    def drag(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'drag' field must be of type 'bool'"
        self._drag = value

    @property
    def x(self):
        """Message field 'x'."""
        return self._x

    @x.setter
    def x(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'x' field must be of type 'float'"
        self._x = value

    @property
    def y(self):
        """Message field 'y'."""
        return self._y

    @y.setter
    def y(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'y' field must be of type 'float'"
        self._y = value

    @property
    def alive(self):
        """Message field 'alive'."""
        return self._alive

    @alive.setter
    def alive(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'alive' field must be of type 'bool'"
        self._alive = value

    @property
    def touch(self):
        """Message field 'touch'."""
        return self._touch

    @touch.setter
    def touch(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'touch' field must be of type 'bool'"
        self._touch = value
