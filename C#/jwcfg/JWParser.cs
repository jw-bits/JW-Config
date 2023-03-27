using System.Collections.Generic;
using System.IO;
using System.Text;

static class JWParser
{
	public sealed class Value
	{	
		public enum EType
		{
			eEmpty = 0,
			eInt,
			eFloat,
			eBool,
			eString
		};

		public Value()
		{
			mType = EType.eEmpty;
			mInt = 0;
			mFloat = 0.0f;
			mBool = false;
		}

		public void set(int i) { mType = EType.eInt; mInt = i; }
		public void set(float f) { mType = EType.eFloat; mFloat = f; }
		public void set(bool b) { mType = EType.eBool; mBool = b; }
		public void set(in string s) { mType = EType.eString; mString = s; }

		public EType getType() { return mType; }
		public int asInt() { return mInt; }
		public float asFloat() { return mFloat; }
		public bool asBool() { return mBool; }
		public ref string asString() { return ref mString; }

		EType mType;
		int mInt;
		float mFloat;
		bool mBool;
		string mString;
	}

     public enum EReturnCode
     {
          eSuccess = 0,
		eInvalidKey,
		eMalformedData,
		eNoValidData,
		eUnexpectedValue
     };

     public static EReturnCode Parse(in string jwBuffer, ref Dictionary<string, Value> jwMap)
	{
		MemoryStream memStream = new MemoryStream(Encoding.UTF8.GetBytes(jwBuffer));
		List<string> strLines = new List<string>();

		using (StreamReader reader = new StreamReader(memStream))
		{
			while (!reader.EndOfStream)
			{
				var s = reader.ReadLine();

				if (IsValid(s))
					strLines.Add(s);
			}
		}

		if (strLines.Count == 0)
			return EReturnCode.eNoValidData;

		jwMap.Clear();

		for (int i = 0; i < strLines.Count; ++i)
		{
			var s = strLines[i];
			int equalIdx = s.IndexOf('=');
			
			if (equalIdx == -1)
				continue;
			
			string keyString = s.Substring(0, equalIdx).Trim();
			string valueString = s.Substring(equalIdx + 1).Trim();

			var v = new Value();

			JWParser.EReturnCode rc = ParseValue(valueString, ref v);

			if (rc != EReturnCode.eSuccess)
				return rc;

			jwMap.Add(keyString, v);
		}

		return EReturnCode.eSuccess;
	}

	public static EReturnCode Parse(string[] linesOfText, ref Dictionary<string, Value> jwMap)
	{
		if (linesOfText == null)
			return EReturnCode.eNoValidData;

		if (linesOfText.Length == 0)
			return EReturnCode.eNoValidData;

		jwMap.Clear();

		for (int i = 0; i < linesOfText.Length; ++i)
		{
			var s = linesOfText[i];
			int equalIdx = s.IndexOf('=');
			
			if (equalIdx == -1)
				continue;
			
			string keyString = s.Substring(0, equalIdx).Trim();
			string valueString = s.Substring(equalIdx + 1).Trim();

			var v = new Value();

			JWParser.EReturnCode rc = ParseValue(valueString, ref v);

			if (rc != EReturnCode.eSuccess)
				return rc;

			jwMap.Add(keyString, v);
		}

		return EReturnCode.eSuccess;
	}

	public static string ToString(Dictionary<string, Value> jwMap)
	{
		if (jwMap.Count == 0)
			return string.Empty;

		var strBuilder = new StringBuilder();

		foreach (var entry in jwMap)
		{
			string line = string.Empty;

			switch(entry.Value.getType())
			{
				case Value.EType.eBool: line = string.Format("{0}={1}", entry.Key, entry.Value.asBool()); break;
				case Value.EType.eInt: line = string.Format("{0}={1}", entry.Key, entry.Value.asInt()); break;
				case Value.EType.eString: line = string.Format("{0}=\"{1}\"", entry.Key, entry.Value.asString()); break;
				case Value.EType.eFloat: line = string.Format("{0}={1}", entry.Key, entry.Value.asFloat()); break;
				default: break;
			}

               strBuilder.AppendLine(line);
          }

		return strBuilder.ToString();
	}

	private static bool IsValid(in string line)
	{
		int eq1 = line.IndexOf('=');
		int eq2 = line.LastIndexOf('=');

		// No equal signs, not valid
		if (eq1 == -1)
			return false;

		// Exactly 1 equal sign, valid
		if (eq1 == eq2)
			return true; 

		// Only way to have 2 or more equal signs is if it is a string.... Check for string formating
		int qIdx = line.Length - 1;

		// First check end of string
		if (line[qIdx] != '"')
			return false;
		else // Need to see if first non-white char is a '"' before next '='
		{
			qIdx = eq1 + 1;
			char ch = ' ';

			while (qIdx < eq2)
			{
				ch = line[qIdx];

				if (char.IsWhiteSpace(ch))
					qIdx++;
				else
					break;
			}

			return ch.Equals('"');
		}		
	}

	static EReturnCode ParseValue(in string str, ref Value jwValue)
	{
		char ch = str[0];

		// Possibily an integer or float
		if (char.IsDigit(ch) || ch == '-' || ch == '.')
		{
			bool foundPeriod = (ch == '.');

			// Look for a period
			if (!foundPeriod)
			{
				int idx = str.IndexOf('.');
				
				if (idx != -1)
					foundPeriod = true;
			}
			
			if (foundPeriod) // It's a float
			{
				float f = 0.0f;
				
				if (float.TryParse(str, out f) == false)
					return EReturnCode.eMalformedData;

				jwValue.set(f);
			}
			else // It's a int
			{
				int i = 0;

				if (int.TryParse(str, out i) == false)
					return EReturnCode.eMalformedData;

				jwValue.set(i);
			}

			return EReturnCode.eSuccess;
		}
		// Possibly a bool
		else if (ch == 't' || ch == 'T' || ch == 'f' || ch == 'F')
		{
			if (str.Length == 4) // Check for truth
			{
				bool R = (str[1] == 'r' || str[1] == 'R');
				bool U = (str[2] == 'u' || str[2] == 'U');
				bool E = (str[3] == 'e' || str[3] == 'E');

				if (R && U && E)
				{
					jwValue.set(true);
						return EReturnCode.eSuccess;
				}
				else
					return EReturnCode.eMalformedData;
			}
			else if (str.Length == 5) // Check for falsehood
			{
				bool A = (str[1] == 'a' || str[1] == 'A');
				bool L = (str[2] == 'l' || str[2] == 'L');
				bool S = (str[3] == 's' || str[3] == 'S');
				bool E = (str[4] == 'e' || str[4] == 'E');

				if (A && L && S && E)
				{
					jwValue.set(false);
						return EReturnCode.eSuccess;
				}
				else
					return EReturnCode.eMalformedData;
			}
		}
		// Check for string
		else if (str[0] == '"' && str[str.Length - 1] == '"' && str.Length >= 3)
		{
			string s = str.Substring(1, str.Length - 2);
			jwValue.set(s);
				return EReturnCode.eSuccess;
		}

		return EReturnCode.eMalformedData;
	}
}