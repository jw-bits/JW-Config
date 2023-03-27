using System;
using System.Collections.Generic;

namespace jwcfg
{
     class Program
     {
          static void Main(string[] args)
          {
               // Visual Studio 2019 expects to read from Debug (on Debug builds) so we go up 2 levels to where the project is
               string jw = System.IO.File.ReadAllText("../../JWTest.txt");
               var jwMap = new Dictionary<string, JWParser.Value>();

               JWParser.Parse(jw, ref jwMap);

               foreach (var entry in jwMap)
               {
                    Console.Write("Key = {0}", entry.Key);

                    switch(entry.Value.getType())
                    {
                         case JWParser.Value.EType.eInt: Console.Write(" Value = {0}", entry.Value.asInt()); break;
                         case JWParser.Value.EType.eFloat: Console.Write(" Value = {0}", entry.Value.asFloat()); break;
                         case JWParser.Value.EType.eBool: Console.Write(" Value = {0}", entry.Value.asBool()); break;
                         case JWParser.Value.EType.eString: Console.Write(" Value = {0}", entry.Value.asString()); break;
                         default: break;
                    }

                    Console.Write("\n");
               }

               string asText = JWParser.ToString(jwMap);
               
               Console.Write("\n ... As text ... \n");
               Console.Write(asText);               

               Console.ReadKey();
          }
     }
}
