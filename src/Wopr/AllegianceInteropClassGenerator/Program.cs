using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace AllegianceInteropClassGenerator
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                Console.WriteLine("Usage: [-m|-i] <filename.h> <outputfilename.h>");
                Console.WriteLine("\t-m = Generate message definitions from messages.h/messageslc.h, etc");
                Console.WriteLine("\t-i = Generate wrapper file from igc.h");
                Console.WriteLine("\tGenerates interop classes from the target messages.h file and writes the generated classes to the output header file.");
                return;
            }

            Program p = new Program();
            p.Run(args[0], args[1], args[2]);
        }

        private void Run(string commandSwitch, string headerFilename, string outputHeaderFilename)
        {
            if (commandSwitch == "-m")
                GenerateMessageWrappers(headerFilename, outputHeaderFilename);

            if(commandSwitch == "-i")
                GenerateIgcWrappers(headerFilename, outputHeaderFilename);

            if (commandSwitch == "-f")
            {
                FixupNullChecks(headerFilename, outputHeaderFilename);
            }
        }

        private void FixupNullChecks(string headerFilename, string outputHeaderFilename)
        {
            string headerFileText = File.ReadAllText(headerFilename);

            // \{\s+(?<return>return gcnew \w+)\(\((?<type>::\w+\s\*)\)\s(?<call>m_instance->\w+\(.*?\))\);\s+\}
            Regex assignmentCallFinder = new Regex(
      "\\{\\s+(?<return>return gcnew \\w+)\\(\\((?<type>::\\w+\\s\\*" +
      ")\\)\\s(?<call>m_instance->\\w+\\(.*?\\))\\);\\s+\\}",
    RegexOptions.Multiline
    | RegexOptions.Singleline
    | RegexOptions.CultureInvariant
    | RegexOptions.Compiled
    );

            /*
{
    ${type} unmanagedValue = (${type}) ${call};

	if (unmanagedValue == nullptr)
		return nullptr;
	else
		${return}(unmanagedValue);
 }
             */

            string replacementString =
      "{\r\n	${type} unmanagedValue = (${type}) ${call};\r\n\r\n	if (unma" +
      "nagedValue == nullptr)\r\n		return nullptr;\r\n	else\r\n		${return" +
      "}(unmanagedValue);\r\n}";

            // Messes up the output, maybe the string is too big for .net to process in one go?
            string result = assignmentCallFinder.Replace(headerFileText, replacementString);

            //string result = headerFileText;

            //foreach (Match m in assignmentCallFinder.Matches(headerFileText))
            //{
            //    string processText = m.Value;
            //    string replaceText = assignmentCallFinder.Replace(processText, replacementString);
            //    result = result.Replace(processText, replaceText);
            //}


            File.WriteAllText(outputHeaderFilename, result);
        }

        private void GenerateIgcWrappers(string headerFilename, string outputHeaderFilename)
        {
            string headerFileText = File.ReadAllText(headerFilename);
            StreamWriter sw = new StreamWriter(outputHeaderFilename, false);
            StreamWriter swCPP = new StreamWriter(outputHeaderFilename.Replace(".h", ".cpp"));

            // $\s*class\s+(?<className>\w+)(\s*:\s*public\s+(?<super>\w+))?\s*\{(?<classBody>.*?)\}\s*;
            // \s*class\s+(?<className>\w+)(\s*:\s*public\s+(?<super>\w+))?.*?\{(?<classBody>.*?)\}\s*;
            // \s*class\s+(?<className>\w+)(\s*:\s*public\s+(?<super>\w+))?.*?\{(?<classBody>.*?)^\s*\}\s*;
            Regex classFinder = new Regex(
                    "\\s*class\\s+(?<className>\\w+)(\\s*:\\s*public\\s+(?<super>" +
                    "\\w+))?.*?\\{(?<classBody>.*?)^\\s*\\}\\s*;",
                RegexOptions.Multiline
                | RegexOptions.Singleline
                | RegexOptions.CultureInvariant
                | RegexOptions.Compiled
                );

            // ^\s*(virtual\s*)?(?<type>\w(\w|\s|\*)*)\s+(?<method>\w+)\((?<signature>.*?)\)((\s*\{.*?\})|(\s*;)|$)

            // $\s*(virtual\s*)?(?<type>\w(\w|\s|\*)*)\s+(?<method>\w+)\((?<signature>.*?)\)
            Regex methodFinder = new Regex(
                    "$\\s*(virtual\\s*)?(?<type>\\w(\\w|\\s|\\*)*)\\s+(?<method>\\w" +
                    "+)\\((?<signature>.*?)\\)",
                RegexOptions.Multiline
                | RegexOptions.CultureInvariant
                | RegexOptions.Compiled
                );

            var classMatches = classFinder.Matches(headerFileText);

            List<String> classNames = new List<string>();

            

            swCPP.WriteLine($@"
#include ""igcWrapper.h""

using namespace System;

namespace AllegianceInterop
{{
");

            sw.WriteLine($@"
#pragma once

#include ""igc.h""

using namespace System;

namespace AllegianceInterop
{{
");
            foreach (Match m in classMatches)
            {
                string className = m.Groups["className"].Value;
                classNames.Add(className.Trim().ToLower());

                if (className != "IstaticIGC")
                    continue;

                sw.Write($@"
    ref class {className}Wrapper;");
            }

            foreach (Match m in classMatches)
            {
                string className = m.Groups["className"].Value;
                string super = m.Groups["super"].Value;
                string classBody = m.Groups["classBody"].Value;

                if (className != "IstaticIGC")
                    continue;

                Console.WriteLine("Processing: " + className);

                // Skip IObject, it's defined in TRef, so we don't really need a subclass of it. 
                if (super.Trim() == "IObject")
                    super = "";

                classBody = CleanClassBody(classBody);

                if (String.IsNullOrWhiteSpace(super) == false)
                {
                    sw.Write($@"
    public ref class {className}Wrapper : {super}Wrapper");
                }
                else
                {
                    sw.Write($@"
    public ref class {className}Wrapper");
                }
                sw.WriteLine($@"
	{{
	public:
		::{className} * m_instance;

	public:
		
		{className}Wrapper(::{className} * instance);");

                swCPP.Write($@"
{className}Wrapper::{className}Wrapper(::{className} * instance)");

                if (String.IsNullOrWhiteSpace(super) == false)
                {
                    swCPP.Write($@" : {super}Wrapper(instance)");
                }


                swCPP.Write($@"
		{{
			m_instance = instance;
		}}");
                foreach (Match methodMatch in methodFinder.Matches(classBody))
                {
                    string type = methodMatch.Groups["type"].Value;
                    string method = methodMatch.Groups["method"].Value;
                    string signature = methodMatch.Groups["signature"].Value;

                    string signatureParams = GetSignatureParameters(headerFileText, signature, classNames);
                    string callParams = GetCallParameters(headerFileText, signature, classNames);

                    // Skip constructors.
                    if (String.IsNullOrWhiteSpace(type) == true)
                        continue;

                    if (type.Contains(";") == true)
                        continue;

                    bool isCommmented = false;
                    if (type.StartsWith("static") == true || signature.Contains("(") == true || signature.Contains(")") == true)
                    {
                        isCommmented = true;
                        sw.Write("/*");
                        swCPP.Write("/*");
                    }

                    bool isString = false;
                    string matchType = type.ToLower().Replace(" ", "");
                    if (matchType.Contains("char*") == true || matchType.Contains("charconst*") == true)
                    {
                        type = "String ^";
                        isString = true;
                    }

                    bool isWrapableClass = false;
                    string testType = type.Trim();
                    if (testType.StartsWith("const ") == true)
                        testType = testType.Substring(6);

                    string unmanagedType = type.Trim();

                    var splitType = testType.Split(new char[] { ' ', '*' });
                    if (classNames.Contains(splitType[0].Trim().ToLower()) == true) // /*splitType.Length == 2 && */headerFileText.IndexOf("class " + splitType[0].Trim(), StringComparison.InvariantCultureIgnoreCase) >= 0)
                    {
                        isWrapableClass = true;
                        type = splitType[0].Trim() + "Wrapper ^";
                    }

                    //Type typeTest = Type.GetType("type");

                        sw.Write($@"
        {type} {method}({signatureParams});");

                    swCPP.Write($@"
{type} {className}Wrapper::{method}({signatureParams})
		{{");
                    if (isString == true)
                    {
                        swCPP.Write($@"
			return gcnew String(m_instance->{method}({callParams}));");
                    }
                    else if (isWrapableClass == true)
                    {
                        swCPP.Write($@"
			return gcnew {type.Split('^')[0].Trim()}((::{unmanagedType.Split(new char[] { '*', ' ' })[0]} *) m_instance->{method}({callParams}));");
                    }
                    else
                    {
                        swCPP.Write($@"
			return m_instance->{method}({callParams});");
                    }

                    swCPP.Write($@"
		}}");

                    if (isCommmented == true)
                    {
                        sw.Write("*/");
                        swCPP.Write("*/");
                    }
                }


                // Close the header class.
                sw.WriteLine($@"
    }};");
                
            }

            sw.WriteLine($@"
}}");

            swCPP.WriteLine($@"
}}");

            sw.Close();
            swCPP.Close();
        }

        private string CleanClassBody(string classBody)
        {
            if (classBody.IndexOf("{") < 0)
                return classBody;

            int braceCount = 0;
            bool foundBlock = false;
            do
            {
                foundBlock = false;

                for (int i = classBody.IndexOf("{"); i < classBody.Length; i++)
                {
                    if (classBody[i] == '{')
                        braceCount++;

                    if(classBody[i] == '}')
                        braceCount--;

                    if (braceCount == 0)
                    {
                        foundBlock = true;
                        classBody = classBody.Substring(0, classBody.IndexOf("{")).Trim() + ";\n" + classBody.Substring(i);
                        break;
                    }
                }

            } while (foundBlock == true && classBody.IndexOf("{") >= 0);

            return classBody;
        }

        private string GetCallParameters(string headerFileText, string signature, List<String> classNames)
        {
            // (?<type>.*?)\s*(?<name>\w+(\s*\[.*?\])?)\s*(,|$|(\=\s*\w+))
            Regex argumentFinder = new Regex(
                    "(?<type>.*?)\\s*(?<name>\\w+(\\s*\\[.*?\\])?)\\s*(,|$|(\\=\\s" +
                     "*\\w+))",
                RegexOptions.CultureInvariant
                | RegexOptions.Compiled
                );

            StringBuilder output = new StringBuilder();

            foreach (Match m in argumentFinder.Matches(signature))
            {
                string type = m.Groups["type"].Value.Trim();
                string name = m.Groups["name"].Value.Trim();

                if (String.IsNullOrWhiteSpace(type) == true) //type.Equals("void", StringComparison.InvariantCultureIgnoreCase) == true)
                    continue;

                name = name.Split('[')[0];

                string testType = type.Trim();
                if (testType.StartsWith("const ", StringComparison.InvariantCultureIgnoreCase) == true)
                    testType = testType.Substring(6);

                string[] typeParts = testType.Split(new char[] { ' ', '*' });
                //if (headerFileText.Contains("class " + typeParts[0]) == true)
                if (classNames.Contains(typeParts[0].Trim().ToLower()) == true)
                    name = name + "->m_instance";

                if (output.Length > 0)
                    output.Append(", ");

                output.Append(name);
            }

            return output.ToString();
        }

        private string GetSignatureParameters(string headerFileText, string signature, List<String> classNames)
        {
            // (?<type>.*?)\s*(?<name>\w+(\s*\[.*?\])?)\s*(,|$|(\=\s*\w+))
            Regex argumentFinder = new Regex(
                    "(?<type>.*?)\\s*(?<name>\\w+(\\s*\\[.*?\\])?)\\s*(,|$|(\\=\\s" +
                     "*\\w+))",
                RegexOptions.CultureInvariant
                | RegexOptions.Compiled
                );

            StringBuilder output = new StringBuilder();

            foreach (Match m in argumentFinder.Matches(signature))
            {
                string type = m.Groups["type"].Value.Trim();
                string name = m.Groups["name"].Value.Trim();

                if (String.IsNullOrWhiteSpace(type) == true) //type.Equals("void", StringComparison.InvariantCultureIgnoreCase) == true)
                    continue;

                if (output.Length > 0)
                    output.Append(", ");

                string testType = type.Trim();
                if (testType.StartsWith("const ", StringComparison.InvariantCultureIgnoreCase) == true)
                    testType = testType.Substring(6);

                string[] typeParts = testType.Split(new char [] { ' ', '*'});
                //if (headerFileText.Contains("class " + typeParts[0]) == true)
                if (classNames.Contains(typeParts[0].Trim().ToLower()) == true)
                    type = testType.Replace(typeParts[0].Trim(), typeParts[0].Trim() + "Wrapper").Replace("*", "^");
                
                output.Append(type + " " + name);
            }

            return output.ToString();
        }

        private void GenerateMessageWrappers(string headerFilename, string outputHeaderFilename)
        {
            string headerFileText = File.ReadAllText(headerFilename);

            // DEFINE_FEDMSG(?<message>.*?)END_FEDMSG
            Regex fedmsgFinder = new Regex(
                  "DEFINE_FEDMSG(?<message>.*?)END_FEDMSG",
                RegexOptions.Multiline
                | RegexOptions.Singleline
                | RegexOptions.CultureInvariant
                | RegexOptions.Compiled
                );

            // \((?<sender>\w+),\s*(?<messageType>.*?),\s*(?<messageID>\d+)\)
            Regex messageHeaderFinder = new Regex(
                 "\\((?<sender>\\w+),\\s*(?<messageType>.*?),\\s*(?<messageID>" +
                 "\\d+)\\)",
               RegexOptions.Multiline
               | RegexOptions.Singleline
               | RegexOptions.CultureInvariant
               | RegexOptions.Compiled
               );

            // (^\s+FM_VAR_ITEM\((?<varitemName>.*?)\s*\)\s*;)|(^\s+(?<type>.*?)\s+(?<name>.*?);)
            Regex propertyFinder = new Regex(
                  "(^\\s+FM_VAR_ITEM\\((?<varitemName>.*?)\\s*\\)\\s*;)|(^\\s+(" +
                  "?<type>.*?)\\s+(?<name>.*?);)",
                RegexOptions.Multiline
                | RegexOptions.CultureInvariant
                | RegexOptions.Compiled
                );

            Dictionary<int, string> messageTypes = new Dictionary<int, string>();
            StreamWriter sw = new StreamWriter(outputHeaderFilename, false);
            
            sw.WriteLine($@"
#pragma once
#include ""ManagedObject.h""

using namespace System;
namespace AllegianceInterop
{{
    
");

            foreach (Match m in fedmsgFinder.Matches(headerFileText))
            {
                string message = m.Groups["message"].Value;

                Match messageHeaderMatch = messageHeaderFinder.Match(message);

                if (messageHeaderMatch.Success == false)
                    continue;

                string sender = messageHeaderMatch.Groups["sender"].Value;
                string messageType = messageHeaderMatch.Groups["messageType"].Value;
                int messageID = Int32.Parse(messageHeaderMatch.Groups["messageID"].Value);

                messageTypes.Add(messageID, "FM_" + sender + "_" + messageType);

                sw.WriteLine($@"
    public ref class FMD_{sender}_{messageType} : public ManagedObject<::FMD_{sender}_{messageType}>
	{{
	public:
		
        FMD_{sender}_{messageType}(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {{
        }}

		FMD_{sender}_{messageType}(");

                bool firstParam = true;

                // Add the constructor for sends
                foreach (Match propertyMatch in propertyFinder.Matches(message))
                {
                    string varitemName = propertyMatch.Groups["varitemName"].Value;
                    string type = propertyMatch.Groups["type"].Value;
                    string name = propertyMatch.Groups["name"].Value;

                    
                    if (firstParam == false)
                        sw.Write(",");
                    else
                        firstParam = false;

                    // If it's a FM_VAR_PARAM, then we will just treat it like a string. Many of these will need to be changed to their correct types.
                    if (String.IsNullOrWhiteSpace(varitemName) == false)
                    {
                        sw.Write($@"
                        String ^ {varitemName}");
                    }
                    else
                    {
                        name = CleanName(name);
                        if (String.IsNullOrWhiteSpace(name) == true)
                            continue;
                        
                        if (name.Contains("[") == true)
                        {
                            sw.Write($@"
                        String ^ {name.Split('[')[0]} /* Originally: {type} {name} */");
                        }
                        else
                        {
                            sw.Write($@"
                        {type} {name}");
                        }
                    }
                        
                }

                sw.Write($@"
            ) 
		{{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, {sender}, {messageType})");

                // Add the constructor body for sends.
                bool pfmStarted = true;
                foreach (Match propertyMatch in propertyFinder.Matches(message))
                {
                    string varitemName = propertyMatch.Groups["varitemName"].Value;
                    string type = propertyMatch.Groups["type"].Value;
                    string name = propertyMatch.Groups["name"].Value;

                    if (String.IsNullOrWhiteSpace(varitemName) == false)
                    {
                        sw.Write($@"
                FM_VAR_PARM(StringToCharArray({varitemName}), CB_ZTS)");

                    }
                    else
                    {
                        if (pfmStarted == true)
                        {
                            sw.WriteLine($@"
            END_PFM_CREATE");

                            pfmStarted = false;
                        }

                        name = CleanName(name);
                        if (String.IsNullOrWhiteSpace(name) == true)
                            continue;

                        if (name.Contains("[") == true)
                        {
                            sw.Write($@"
             strcpy(pfm->{name.Split('[')[0]}, StringToCharArray({name.Split('[')[0]})); /* Originally: pfm->{name} = {name}; */");
                        }
                        else
                        {
                            sw.Write($@"
             pfm->{name} = {name};");
                        }
                    }

                }

                if (pfmStarted == true)
                {
                    sw.WriteLine($@"
            END_PFM_CREATE");
                }
                
                sw.WriteLine($@"

            this->m_Instance = pfm;
        }}
");

                // Add the properties.
                foreach (Match propertyMatch in propertyFinder.Matches(message))
                {
                    string varitemName = propertyMatch.Groups["varitemName"].Value;
                    string type = propertyMatch.Groups["type"].Value;
                    string name = propertyMatch.Groups["name"].Value;

                    if (String.IsNullOrWhiteSpace(varitemName) == false)
                    {
                        sw.WriteLine($@"

        property String ^ {varitemName}
		{{
		public:
			String ^ get()
			{{
				return gcnew String(FM_VAR_REF(m_Instance, {varitemName})); 
			}}
		}}

");
                    }
                    else
                    {
                        name = CleanName(name);
                        if (String.IsNullOrWhiteSpace(name) == true)
                            continue;

                        if (name.Contains("[") == true)
                        {
                            sw.Write($@"
        property String ^ {name.Split('[')[0]} /* Originally: {type} {name} */
		{{
		public:
			String ^ get()
			{{
				return gcnew String(m_Instance->{name.Split('[')[0]}); 
			}}
		}}
");

                        }
                        else
                        {
                            sw.Write($@"
        property {type} {name}
		{{
		public:
			{type} get()
			{{
				return m_Instance->{name}; 
			}}
		}}
");
                        }
                    }
                }

                // Close off the class.
                sw.WriteLine($@"
	}};
");


            }

            sw.WriteLine("");
            sw.WriteLine("\tpublic enum MessageTypes {");

            foreach (int key in messageTypes.Keys)
            {
                sw.WriteLine($"\t\t{messageTypes[key]} = {key},");
            }

            sw.WriteLine("\t};");


            // Close off the namespace
            sw.WriteLine("}");

            sw.Close();
        }

        public string CleanName(string name)
        {
            if (name.Trim().StartsWith("//") == true)
                return string.Empty;

            return name.Split(':')[0].Trim();

        }
    }
}
