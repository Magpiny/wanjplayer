#ifndef __STRING_UTILS_HPP
#define __STRING_UTILS_HPP

#include <wx/wx.h>
#include <wx/arrstr.h>
#include <wx/tokenzr.h>
#include <vector>

namespace utils {

// String utilities
class StringUtils {
public:
    // Basic string operations
    static wxString Trim(const wxString& str);
    static wxString TrimLeft(const wxString& str);
    static wxString TrimRight(const wxString& str);
    static wxString ToLower(const wxString& str);
    static wxString ToUpper(const wxString& str);
    static wxString Capitalize(const wxString& str);
    static wxString TitleCase(const wxString& str);
    
    // String testing
    static bool IsEmpty(const wxString& str);
    static bool IsWhitespace(const wxString& str);
    static bool IsNumeric(const wxString& str);
    static bool IsAlphabetic(const wxString& str);
    static bool IsAlphaNumeric(const wxString& str);
    static bool StartsWith(const wxString& str, const wxString& prefix);
    static bool EndsWith(const wxString& str, const wxString& suffix);
    static bool Contains(const wxString& str, const wxString& substring);
    static bool ContainsIgnoreCase(const wxString& str, const wxString& substring);
    
    // String splitting and joining
    static wxArrayString Split(const wxString& str, const wxString& delimiter);
    static wxArrayString SplitLines(const wxString& str);
    static wxArrayString SplitWords(const wxString& str);
    static wxString Join(const wxArrayString& strings, const wxString& separator);
    static wxString Join(const std::vector<wxString>& strings, const wxString& separator);
    
    // String replacement and modification
    static wxString Replace(const wxString& str, const wxString& old_val, const wxString& new_val);
    static wxString ReplaceAll(const wxString& str, const wxString& old_val, const wxString& new_val);
    static wxString ReplaceIgnoreCase(const wxString& str, const wxString& old_val, const wxString& new_val);
    static wxString RemoveAll(const wxString& str, const wxString& to_remove);
    static wxString RemoveCharacters(const wxString& str, const wxString& chars_to_remove);
    static wxString KeepOnly(const wxString& str, const wxString& chars_to_keep);
    
    // String padding and alignment
    static wxString PadLeft(const wxString& str, size_t width, wxChar pad_char = ' ');
    static wxString PadRight(const wxString& str, size_t width, wxChar pad_char = ' ');
    static wxString PadCenter(const wxString& str, size_t width, wxChar pad_char = ' ');
    static wxString Repeat(const wxString& str, size_t count);
    
    // String searching and indexing
    static int FindFirst(const wxString& str, const wxString& substring);
    static int FindLast(const wxString& str, const wxString& substring);
    static int FindIgnoreCase(const wxString& str, const wxString& substring);
    static int CountOccurrences(const wxString& str, const wxString& substring);
    static int CountOccurrences(const wxString& str, wxChar ch);
    
    // String extraction
    static wxString Left(const wxString& str, size_t count);
    static wxString Right(const wxString& str, size_t count);
    static wxString Mid(const wxString& str, size_t start, size_t length = wxString::npos);
    static wxString Between(const wxString& str, const wxString& start_delimiter, const wxString& end_delimiter);
    static wxString BeforeFirst(const wxString& str, const wxString& delimiter);
    static wxString AfterFirst(const wxString& str, const wxString& delimiter);
    static wxString BeforeLast(const wxString& str, const wxString& delimiter);
    static wxString AfterLast(const wxString& str, const wxString& delimiter);
    
    // String validation and formatting
    static bool IsValidEmail(const wxString& email);
    static bool IsValidUrl(const wxString& url);
    static bool IsValidFilename(const wxString& filename);
    static wxString SanitizeFilename(const wxString& filename);
    static wxString EscapeSpecialChars(const wxString& str);
    static wxString UnescapeSpecialChars(const wxString& str);
    
    // Encoding and escaping
    static wxString EscapeXml(const wxString& str);
    static wxString UnescapeXml(const wxString& str);
    static wxString EscapeJson(const wxString& str);
    static wxString UnescapeJson(const wxString& str);
    static wxString EscapeRegex(const wxString& str);
    static wxString UrlEncode(const wxString& str);
    static wxString UrlDecode(const wxString& str);
    
    // String comparison
    static int Compare(const wxString& str1, const wxString& str2);
    static int CompareIgnoreCase(const wxString& str1, const wxString& str2);
    static int CompareNatural(const wxString& str1, const wxString& str2);
    static bool Equals(const wxString& str1, const wxString& str2);
    static bool EqualsIgnoreCase(const wxString& str1, const wxString& str2);
    
    // String conversion
    static long ToLong(const wxString& str, long default_value = 0);
    static double ToDouble(const wxString& str, double default_value = 0.0);
    static bool ToBool(const wxString& str, bool default_value = false);
    static wxString FromLong(long value);
    static wxString FromDouble(double value, int precision = -1);
    static wxString FromBool(bool value);
    
    // String formatting
    static wxString FormatSize(long long bytes);
    static wxString FormatNumber(long long number);
    static wxString FormatPercent(double percent, int decimals = 1);
    static wxString FormatList(const wxArrayString& items, const wxString& separator = ", ", const wxString& last_separator = " and ");
    
    // Advanced operations
    static wxString Reverse(const wxString& str);
    static wxString Shuffle(const wxString& str);
    static wxString GenerateRandom(size_t length, const wxString& charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    static wxString ToBase64(const wxString& str);
    static wxString FromBase64(const wxString& base64_str);
    
    // String similarity and distance
    static double CalculateSimilarity(const wxString& str1, const wxString& str2);
    static int LevenshteinDistance(const wxString& str1, const wxString& str2);
    static wxString GetCommonPrefix(const wxString& str1, const wxString& str2);
    static wxString GetCommonSuffix(const wxString& str1, const wxString& str2);
    
    // Line operations
    static wxArrayString WrapLines(const wxString& text, size_t line_width);
    static wxString UnwrapLines(const wxString& text);
    static wxString IndentLines(const wxString& text, const wxString& indent = "    ");
    static wxString UnindentLines(const wxString& text);
    
    // Hash and checksum
    static wxString GetMD5Hash(const wxString& str);
    static wxString GetSHA1Hash(const wxString& str);
    static wxUint32 GetCRC32(const wxString& str);
    
private:
    // Helper functions
    static bool IsWhitespaceChar(wxChar ch);
    static bool IsValidEmailChar(wxChar ch);
    static bool IsValidFilenameChar(wxChar ch);
    static wxString GenerateRandomString(size_t length, const wxString& charset);
    static double JaccardSimilarity(const wxString& str1, const wxString& str2);
};

}

#endif // __STRING_UTILS_HPP