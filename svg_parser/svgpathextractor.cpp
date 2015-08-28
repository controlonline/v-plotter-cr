// vim: enc=utf8:fenc=utf8:textwidth=80:spelllang=en:spell

#include "svgpathextractor.h"

#include <QtDebug>
#include <QXmlSimpleReader>

// Constructor
CSvgPathExtractor::CSvgPathExtractor () {
}

// Destructor
CSvgPathExtractor::~CSvgPathExtractor () {
}

// Parse the given SVG 1.2 tiny data and return the included paths as a QPainterPath
QPainterPath CSvgPathExtractor::parse (const QByteArray & svg) {
	// XML reader
	QXmlSimpleReader reader;

	// Add a SVG path handler to the reader
	CSvgPathExtractor::CSvgPathHandler handler;
	reader.setContentHandler (&handler);
	reader.setDTDHandler (&handler);
	reader.setDeclHandler (&handler);
	reader.setErrorHandler (&handler);
	reader.setLexicalHandler (&handler);

	// Set SVG data as input source
	QXmlInputSource src;
	src.setData (svg);

	// Start parsing data
	reader.parse (&src, false);

	return handler.globalPath ();
}


// Constructor
CSvgPathExtractor::CSvgPathHandler::CSvgPathHandler () : QXmlDefaultHandler () {
}

// Destructor
CSvgPathExtractor::CSvgPathHandler::~CSvgPathHandler () {
}

// Called when a new xml element is encountered while parsing
// Return true if no error occurs, false otherwise
bool CSvgPathExtractor::CSvgPathHandler::startElement (
		const QString & /*namespaceURI*/,
		const QString & /*localName*/,
		const QString & qName,
		const QXmlAttributes & atts) {
	// Considere only path elements
	if (!qName.compare ("path")) {
		// Get path data
		QString data = atts.value ("d");

		// Parse path data
		if (!parse (data)) {
			qWarning() << "SVG handling error : \"path data parsing error\"";
		}
	}

	return true;
}

// Error handling
bool CSvgPathExtractor::CSvgPathHandler::error (
		const QXmlParseException & exception) {
	qWarning() << "SVG handling error on line" << exception.lineNumber()
		<< ", column" << exception.columnNumber() << ":"
		<< exception.message();

	return false;
}
bool CSvgPathExtractor::CSvgPathHandler::fatalError (
		const QXmlParseException & exception) {
	qWarning() << "SVG handling fatal error on line" << exception.lineNumber()
		<< ", column" << exception.columnNumber() << ":"
		<< exception.message();

	return false;
}
bool CSvgPathExtractor::CSvgPathHandler::warning (
		const QXmlParseException & exception) {
	qWarning() << "SVG handling warning on line" << exception.lineNumber()
		<< ", column" << exception.columnNumber() << ":"
		<< exception.message();

	return true;
}

// Return the QPainterPath containing all SVG paths
// Must be call after parsing !
QPainterPath CSvgPathExtractor::CSvgPathHandler::globalPath () {
	return _global_path;
}

// Parse path data and add the corresponding QPainterPath to the global one
// Return true if parsing was successful, false otherwise
// SVG 1.2 tiny paths are supported
bool CSvgPathExtractor::CSvgPathHandler::parse (QString & data) {
	// SVG commands identifiers
	// See http://www.w3.org/TR/SVGMobile12/paths.html for SVG 1.2 tiny path spec
	static QChar moveto_abs ('M'), moveto_rel ('m');
	static QChar closepath1 ('Z'), closepath2 ('z');
	static QChar lineto_abs ('L'), lineto_rel ('l');
	static QChar hline_abs ('H'), hline_rel ('h');
	static QChar vline_abs ('V'), vline_rel ('v');
	static QChar curveto_abs ('C'), curveto_rel ('c');
	static QChar smoothcurveto_abs ('S'), smoothcurveto_rel ('s');
	static QChar quadcurveto_abs ('Q'), quadcurveto_rel ('q');
	static QChar smoothquadcurveto_abs ('T'), smoothquadcurveto_rel ('t');

	// Store the last command used and the current one
	QChar cmd, last_cmd; 

	// Store the last parsed values
	// x/y = end position
	// x1/y1 and x2/y2 = bezier control points
	float x, y, x1, y1, x2, y2;

	// QPainterPath corresponding to the one beeing parsed
	QPainterPath path;

	// Cleanup data
	data.replace (QChar (','), QChar (' '));

	// Parsing
	int i = 0;
	QChar c;
	while (i < data.size ()) {
		last_cmd = cmd;
		cmd = data.at (i);
		++i;

		if (cmd.isSpace ()) {
			continue;
		} else if (cmd == moveto_abs) {
			x = parseFloat (data, &i);
			y = parseFloat (data, &i);
			path.moveTo (x, y);
		} else if (cmd == moveto_rel) {
			x = parseFloat (data, &i) + x;
			y = parseFloat (data, &i) + y;
			path.moveTo (x, y);
		} else if (cmd == closepath1 || cmd == closepath2) {
			path.closeSubpath ();
			break;
		} else if (cmd == lineto_abs) {
			x = parseFloat (data, &i);
			y = parseFloat (data, &i);
			path.lineTo (x, y);
		} else if (cmd == lineto_rel) {
			x = parseFloat (data, &i) + x;
			y = parseFloat (data, &i) + y;
			path.lineTo (x, y);
		} else if (cmd == hline_abs) {
			x = parseFloat (data, &i);
			y = y;
			path.lineTo (x, y);
		} else if (cmd == hline_rel) {
			x = parseFloat (data, &i) + x;
			y = y;
			path.lineTo (x, y);
		} else if (cmd == vline_abs) {
			x = x;
			y = parseFloat (data, &i);
			path.lineTo (x, y);
		} else if (cmd == vline_rel) {
			x = x;
			y = parseFloat (data, &i) + y;
			path.lineTo (x, y);
		} else if (cmd == curveto_abs) {
			x1 = parseFloat (data, &i);
			y1 = parseFloat (data, &i);
			x2 = parseFloat (data, &i);
			y2 = parseFloat (data, &i);
			x = parseFloat (data, &i);
			y = parseFloat (data, &i);
			path.cubicTo (x1, y1, x2, y2, x, y);
		} else if (cmd == curveto_rel) {
			x1 = parseFloat (data, &i) + x;
			y1 = parseFloat (data, &i) + y;
			x2 = parseFloat (data, &i) + x;
			y2 = parseFloat (data, &i) + y;
			x = parseFloat (data, &i) + x;
			y = parseFloat (data, &i) + y;
			path.cubicTo (x1, y1, x2, y2, x, y);
		} else if (cmd == smoothcurveto_abs) {
			if (last_cmd == curveto_abs
				|| last_cmd == curveto_rel
				|| last_cmd == smoothcurveto_abs
				|| last_cmd == smoothcurveto_rel) {
				x1 = 2 * x - x2;
				y1 = 2 * y - y2;
			} else {
				x1 = x;
				y1 = y;
			}
			x2 = parseFloat (data, &i);
			y2 = parseFloat (data, &i);
			x = parseFloat (data, &i);
			y = parseFloat (data, &i);
			path.cubicTo (x1, y1, x2, y2, x, y);
		} else if (cmd == smoothcurveto_rel) {
			if (last_cmd == curveto_abs
				|| last_cmd == curveto_rel
				|| last_cmd == smoothcurveto_abs
				|| last_cmd == smoothcurveto_rel) {
				x1 = 2 * x - x2;
				y1 = 2 * y - y2;
			} else {
				x1 = x;
				y1 = y;
			}
			x2 = parseFloat (data, &i) + x;
			y2 = parseFloat (data, &i) + y;
			x = parseFloat (data, &i) + x;
			y = parseFloat (data, &i) + y;
			path.cubicTo (x1, y1, x2, y2, x, y);
		} else if (cmd == quadcurveto_abs) {
			x1 = parseFloat (data, &i);
			y1 = parseFloat (data, &i);
			x = parseFloat (data, &i);
			y = parseFloat (data, &i);
			path.quadTo (x1, y1, x, y);
		} else if (cmd == quadcurveto_rel) {
			x1 = parseFloat (data, &i) + x;
			y1 = parseFloat (data, &i) + y;
			x = parseFloat (data, &i) + x;
			y = parseFloat (data, &i) + y;
			path.quadTo (x1, y1, x, y);
		} else if (cmd == smoothquadcurveto_abs) {
			if (last_cmd == quadcurveto_abs
				|| last_cmd == quadcurveto_rel
				|| last_cmd == smoothquadcurveto_abs
				|| last_cmd == smoothquadcurveto_rel) {
				x1 = 2 * x - x1;
				y1 = 2 * y - y1;
			} else {
				x1 = x;
				y1 = y;
			}
			x = parseFloat (data, &i);
			y = parseFloat (data, &i);
			path.quadTo (x1, y1, x, y);
		} else if (cmd == smoothquadcurveto_rel) {
			if (last_cmd == quadcurveto_abs
				|| last_cmd == quadcurveto_rel
				|| last_cmd == smoothquadcurveto_abs
				|| last_cmd == smoothquadcurveto_rel) {
				x1 = 2 * x - x1;
				y1 = 2 * y - y1;
			} else {
				x1 = x;
				y1 = y;
			}
			x = parseFloat (data, &i) + x;
			y = parseFloat (data, &i) + y;
			path.quadTo (x1, y1, x, y);
		} else if (cmd.isDigit () || cmd == QChar ('-') || cmd == QChar ('.')) {
			// A new number begin
			// In this case, multiple parameters tuples are specified for the last command
			
			// We decrement i to reparse data correctly
			--i;

			// Handle extra parameters
			if (last_cmd == moveto_abs) {
				cmd = lineto_abs;
			} else if (last_cmd == moveto_rel) {
				cmd = lineto_rel;
			} else if (last_cmd == closepath1 || last_cmd == closepath2) {
				// Error
				qWarning() << "SVG handling error : \"extra parameters after close path command\"";
				return false;
			} else if (last_cmd == lineto_abs
					|| last_cmd == lineto_rel
					|| last_cmd == hline_abs
					|| last_cmd == hline_rel
					|| last_cmd == vline_abs
					|| last_cmd == vline_rel
					|| last_cmd == curveto_abs
					|| last_cmd == curveto_rel
					|| last_cmd == smoothcurveto_abs
					|| last_cmd == smoothcurveto_rel
					|| last_cmd == quadcurveto_abs
					|| last_cmd == quadcurveto_rel
					|| last_cmd == smoothquadcurveto_abs
					|| last_cmd == smoothquadcurveto_rel) {
				cmd = last_cmd;
			}
		} else {
			// Error
			qWarning() << "SVG handling error : \"unknown `" << cmd << "' path command\"";
			return false;
		}
	}

	// Add the new subpath to the global path
	_global_path.addPath (path);

	return true;
}

// Parse a float value starting at data [i]
// The value 'i' will be updated to the new current position
float CSvgPathExtractor::CSvgPathHandler::parseFloat (const QString & data, int* i) {
	static QChar minus ('-');
	static QChar dec ('.');

	QString f;

	// Discards spaces
	while (*i < data.size () && data.at (*i).isSpace ()) {
		++(*i);
	}

	// Parse a minus
	if (*i < data.size () && data.at (*i) == minus) {
		f.append (minus);
		++(*i);
	}

	// Parse data before decimal separator
	while (*i < data.size () && data.at (*i).isDigit ()) {
		f.append (data.at (*i));
		++(*i);
	}

	// Parse decimal separator
	if (*i < data.size () && data.at (*i) == dec) {
		f.append (dec);
		++(*i);

		// Parse data after decimal separator
		while (*i < data.size () && data.at (*i).isDigit ()) {
			f.append (data.at (*i));
			++(*i);
		}
	}

	return f.toFloat ();
}

