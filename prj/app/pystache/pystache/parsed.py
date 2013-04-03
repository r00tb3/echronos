# coding: utf-8

"""
Exposes a class that represents a parsed (or compiled) template.

"""


class ParsedTemplate(object):

    def __init__(self):
        self._parse_tree = []

    def __repr__(self):
        return repr(self._parse_tree)

    def add(self, node):
        """
        Arguments:

          node: a unicode string or node object instance.  A node object
            instance must have a `render(engine, stack)` method that
            accepts a RenderEngine instance and a ContextStack instance and
            returns a unicode string.

        """
        self._parse_tree.append(node)

    def render(self, engine, context):
        """
        Returns: a string of type unicode.

        """
        # We avoid use of the ternary operator for Python 2.4 support.
        def get_unicode(val):
            if type(val) is str:
                return val
            return val.render(engine, context)
        parts = list(map(get_unicode, self._parse_tree))
        s = ''.join(parts)

        return str(s)
