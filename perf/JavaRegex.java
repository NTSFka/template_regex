
import java.util.List;
import java.util.ArrayList;
import java.io.FileInputStream;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.IOException;
import java.util.regex.Pattern;

class JavaRegex
{
    /**
     * @brief Test interface.
     */
    public static interface ITest
    {
        public boolean call(final String input);
    }


    /**
     * @brief Identifier test.
     */
    public static class IdentifierTest implements ITest
    {
        final Pattern pattern = Pattern.compile("^[a-zA-Z_][a-zA-Z0-9_]*$");

        public boolean call(final String input)
        {
            return pattern.matcher(input).matches();
        }
    }


    /**
     * @brief Date test.
     */
    public static class DateTest implements ITest
    {
        final Pattern pattern = Pattern.compile("^[0-9][0-9]?/[0-9][0-9]?/[0-9][0-9][0-9][0-9]$");

        public boolean call(final String input)
        {
            return pattern.matcher(input).matches();
        }
    }


    /**
     * @brief Float number test.
     */
    public static class FloatTest implements ITest
    {
        final Pattern pattern = Pattern.compile("^[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?$");

        public boolean call(final String input)
        {
            return pattern.matcher(input).matches();
        }
    }


    /**
     * @brief Print Regex header.
     *
     * @param name
     */
    public static void printHeader(final String name)
    {
        System.out.println("---- " + name + " ----");
    }


    /**
     * @brief Perform speed test.
     *
     * @param fun  Test function.
     * @param data Test data.
     *
     * @return
     */
    public static void doTest(final String name, ITest fun, final List<String> data)
    {
        System.out.print(String.format("%20s ... ", name));

        long count = 0;
        String errinput = null;

        long start = System.currentTimeMillis();

        for (String input : data)
        {
            if (!fun.call(input))
            {
                errinput = input;
                break;
            }

            ++count;
        }

        long end = System.currentTimeMillis();

        // Get elapsed time
        long time = end - start;

        // Print result
        System.out.print(String.format("%d / %d: %7d ms", count, data.size(), time));

        if (errinput != null)
            System.out.print(" '" + errinput + "'");

        System.out.println();
    }


    /**
     * @brief Read lines from file.
     *
     * @param filename Source file.
     *
     * @return String list.
     */
    public static final List<String> readLines(final String filename)
    {
        List<String> lines = null;
        InputStream fis = null;
        BufferedReader br = null;
        String line = null;

        try
        {
            fis = new FileInputStream(filename);
            br = new BufferedReader(new InputStreamReader(fis));

            long start = System.currentTimeMillis();

            lines = new ArrayList<String>();

            while ((line = br.readLine()) != null)
            {
                lines.add(line);
            }

            long end = System.currentTimeMillis();

            System.out.println("Load '" + filename + "': " + (end - start) + " ms");
        }
        catch (IOException e)
        {
            System.err.println(e);
            System.exit(0);
        }
        finally
        {
            try { if (br != null) br.close(); } catch (IOException e) {}
            br = null;
            fis = null;
        }

        return lines;
    }


    /**
     * @brief Main function.
     *
     * @param args
     */
    public static void main(final String[] args)
    {
        final String dir = args.length > 0 ? args[0] + "/" : "./";

        final List<String> identifiers = readLines(dir + "identifiers.txt");
        final List<String> dates       = readLines(dir + "dates.txt");
        final List<String> floats      = readLines(dir + "floats.txt");

        printHeader("Java Regex");

        doTest("identifier", new IdentifierTest(), identifiers);
        doTest("date", new DateTest(), dates);
        doTest("float", new FloatTest(), floats);
    }
}

