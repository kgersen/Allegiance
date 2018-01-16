using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace PigJSDocGenerator
{
    class Program
    {
        static int Main(string[] args)
        {
            if (args.Length != 3)
            {
                Console.WriteLine("Usage:");
                Console.WriteLine("PigJSDocGenerator <input .idl file> <output .js file> <output .d.ts file>");
                return -1;
            }

            Program program = new Program();
            return program.Run(args[0], args[1], args[2]);

        }

        private int Run(string inputIdlFile, string outputJsFile, string outputTsFile)
        {
            StreamReader streamReader = new StreamReader(inputIdlFile);
            string idlContents = streamReader.ReadToEnd();

            StreamWriter streamWriterJS = new StreamWriter(outputJsFile, false);
            WriteJSDocHeader(streamWriterJS);

            StreamWriter streamWriterTS = new StreamWriter(outputTsFile, false);
            WriteTSDocHeader(streamWriterTS);

            FindEnumBlocksAndWriteToFile(streamWriterJS, idlContents);

            FindInterfaceBlocksAndWriteToFile(streamWriterTS, idlContents);
            
            streamWriterJS.Close();
            streamWriterTS.Close();

            return 0;
        }

        private void WriteTSDocHeader(StreamWriter streamWriterTS)
        {
            throw new NotImplementedException();
        }

        private void FindInterfaceBlocksAndWriteToFile(StreamWriter streamWriter, string idlContents)
        {
            // (?<block>\[.*?\].*?interface.*?\{.*?\})
            Regex interfaceFinder = new Regex(
                  "(?<block>\\[.*?\\].*?interface.*?\\{.*?\\})",
                RegexOptions.IgnoreCase
                | RegexOptions.Multiline
                | RegexOptions.Singleline
                | RegexOptions.CultureInvariant
                | RegexOptions.Compiled
                );

            // \[.*?helpstring\(\"(?<helpstring>.*?)\"\).*?\].*?interface\s+(?<name>.*?)\s+.*?\{(?<lines>.*?)\}
            Regex elementFinder = new Regex(
                  "\\[.*?helpstring\\(\\\"(?<helpstring>.*?)\\\"\\).*?\\].*?int" +
                  "erface\\s+(?<name>.*?)\\s+.*?\\{(?<lines>.*?)\\}",
                RegexOptions.IgnoreCase
                | RegexOptions.Multiline
                | RegexOptions.Singleline
                | RegexOptions.CultureInvariant
                | RegexOptions.Compiled
                );

            // \[propget.*?helpstring\(\"(?<helpstring>.*?)\"\).*?\].*?HRESULT(?<name>.*?)\((?<parameters>.*?)\);
            Regex propertyFinder = new Regex(
                  "\\[propget.*?helpstring\\(\\\"(?<helpstring>.*?)\\\"\\).*?\\]" +
                  ".*?HRESULT(?<name>.*?)\\((?<parameters>.*?)\\);",
                RegexOptions.IgnoreCase
                | RegexOptions.Multiline
                | RegexOptions.Singleline
                | RegexOptions.CultureInvariant
                | RegexOptions.Compiled
                );

            foreach (Match enumMatch in interfaceFinder.Matches(idlContents))
            {
                Match elementMatch = elementFinder.Match(enumMatch.Groups["block"].Value);
                if (elementMatch.Success == true)
                {
                    string helpstring = elementMatch.Groups["helpstring"].Value;
                    string name = elementMatch.Groups["name"].Value;

                    string[] enumLines = enumValueFinder.Matches(elementMatch.Groups["enumLines"].Value).OfType<Match>().Select(p => p.Groups["enumValue"].Value).ToArray();

                    WriteJSDocEnum(streamWriter, helpstring, enumName, enumLines);
                }
            }
        }

        private void FindEnumBlocksAndWriteToFile(StreamWriter streamWriter, string idlContents)
        {
            // (?<enumBlock>\[.*?\].*?typedef\s+\[v1_enum\]\s+enum.*?\{.*?\})
            Regex enumBlockFinder = new Regex(
                  "(?<enumBlock>\\[.*?\\].*?typedef\\s+\\[v1_enum\\]\\s+enum.*?" +
                  "\\{.*?\\})",
                RegexOptions.IgnoreCase
                | RegexOptions.Multiline
                | RegexOptions.Singleline
                | RegexOptions.CultureInvariant
                | RegexOptions.Compiled
                );

            // \[.*?helpstring\(\"(?<helpstring>.*?)\"\).*?\].*?typedef\s+\[v1_enum\]\s+enum\s+(?<enumName>.*?)\s+\{(?<enumLines>.*?)\}
            Regex enumElementFinder = new Regex(
                  "\\[.*?helpstring\\(\\\"(?<helpstring>.*?)\\\"\\).*?\\].*?typ" +
                  "edef\\s+\\[v1_enum\\]\\s+enum\\s+(?<enumName>.*?)\\s+\\{(?<e" +
                  "numLines>.*?)\\}",
                RegexOptions.IgnoreCase
                | RegexOptions.Multiline
                | RegexOptions.Singleline
                | RegexOptions.CultureInvariant
                | RegexOptions.Compiled
                );

            // \s*(?<enumValue>.*?)(\s*=.*?)?,
            Regex enumValueFinder = new Regex(
                  "\\s*(?<enumValue>.*?)(\\s*=.*?)?,",
                RegexOptions.IgnoreCase
                | RegexOptions.Multiline
                | RegexOptions.CultureInvariant
                | RegexOptions.Compiled
                );

            foreach (Match enumMatch in enumBlockFinder.Matches(idlContents))
            {
                Match elementMatch = enumElementFinder.Match(enumMatch.Groups["enumBlock"].Value);
                if (elementMatch.Success == true)
                {
                    string helpstring = elementMatch.Groups["helpstring"].Value;
                    string enumName = elementMatch.Groups["enumName"].Value;

                    string[] enumLines = enumValueFinder.Matches(elementMatch.Groups["enumLines"].Value).OfType<Match>().Select(p => p.Groups["enumValue"].Value).ToArray();

                    WriteJSDocEnum(streamWriter, helpstring, enumName, enumLines);
                }
            }
        }

        private void WriteJSDocEnum(StreamWriter streamWriter, string helpstring, string enumName, string[] enumLines)
        {
            streamWriter.WriteLine(@"////////////////////////////////////////////////////////////////////////////////////////////////////");
            streamWriter.WriteLine($"// {enumName} Enum");
            streamWriter.WriteLine("");
            streamWriter.WriteLine($"Com.{enumName} = function () {{ }};");
            streamWriter.WriteLine("");

            foreach (string enumLine in enumLines)
            {
                if (enumLine.Length == 0)
                    continue;

                streamWriter.WriteLine($"Com.{enumName}.{enumLine} = {enumLine};");
            }

            streamWriter.WriteLine("");
        }

        private void WriteJSDocHeader(StreamWriter streamWriter)
        {
            streamWriter.WriteLine(@"
// ***
// ** 
// *** This is a generated file. Please do not modify this file by hand or your changes will be lost.
// ***
// ***
//
// These values bridge COM Enums and objects defined in PigsIDL into JS compatible code that can be used for intellisense.
// Include this .js file at the top of your .pig file. Include the .d.ts in your .js with a <reference> element to enable intellisense.


// Com is the top level namespace. All COM based objects and enums will go under this namespace. This will avoid collisions
// from JS with global COM variables like 'PigState' which has the same name as it's enum. 
var Com = function () { };


");

        }
    }
}
