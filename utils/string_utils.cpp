#include "string_utils.hpp"
#include <wx/regex.h>
#include <wx/base64.h>
#include <algorithm>
#include <random>
#include <sstream>

namespace utils {

// Basic string operations
wxString StringUtils::Trim(const wxString& str)
{
    wxString trimmed = str;
    trimmed.Trim(true);
    trimmed.Trim(false);
    return trimmed;
}

wxString StringUtils::TrimLeft(const wxString& str)
{
    wxString trimmed = str;
    trimmed.Trim(false);
    return trimmed;
}

wxString StringUtils::TrimRight(const wxString& str)
{
    wxString trimmed = str;
    trimmed.Trim(true);
    return trimmed;
}

wxString StringUtils::ToLower(const wxString& str)
{
    return str.Lower();
}

wxString StringUtils::ToUpper(const wxString& str)
{
    return str.Upper();
}

wxString StringUtils::Capitalize(const wxString& str)
{
    if (str.IsEmpty()) return str;
    
    wxString result = str.Lower();
    result[0] = wxToupper(result[0]);
    return result;
}

wxString StringUtils::TitleCase(const wxString& str)
{
    wxString result = str.Lower();
    bool capitalize_next = true;
    
    for (size_t i = 0; i < result.Length(); i++) {
        if (IsWhitespaceChar(result[i])) {
            capitalize_next = true;
        } else if (capitalize_next) {
            result[i] = wxToupper(result[i]);
            capitalize_next = false;
        }
    }
    
    return result;
}

// String testing
bool StringUtils::IsEmpty(const wxString& str)
{
    return str.IsEmpty();
}

bool StringUtils::IsWhitespace(const wxString& str)
{
    for (size_t i = 0; i < str.Length(); i++) {
        if (!IsWhitespaceChar(str[i])) {
            return false;
        }
    }
    return true;
}

bool StringUtils::IsNumeric(const wxString& str)
{
    if (str.IsEmpty()) return false;
    
    for (size_t i = 0; i < str.Length(); i++) {
        if (!wxIsdigit(str[i]) && str[i] != '.' && str[i] != '-' && str[i] != '+') {
            return false;
        }
    }
    return true;
}

bool StringUtils::IsAlphabetic(const wxString& str)
{
    if (str.IsEmpty()) return false;
    
    for (size_t i = 0; i < str.Length(); i++) {
        if (!wxIsalpha(str[i])) {
            return false;
        }
    }
    return true;
}

bool StringUtils::IsAlphaNumeric(const wxString& str)
{
    if (str.IsEmpty()) return false;
    
    for (size_t i = 0; i < str.Length(); i++) {
        if (!wxIsalnum(str[i])) {
            return false;
        }
    }
    return true;
}

bool StringUtils::StartsWith(const wxString& str, const wxString& prefix)
{
    return str.StartsWith(prefix);
}

bool StringUtils::EndsWith(const wxString& str, const wxString& suffix)
{
    return str.EndsWith(suffix);
}

bool StringUtils::Contains(const wxString& str, const wxString& substring)
{
    return str.Contains(substring);
}

bool StringUtils::ContainsIgnoreCase(const wxString& str, const wxString& substring)
{
    return str.Upper().Contains(substring.Upper());
}

// String splitting and joining
wxArrayString StringUtils::Split(const wxString& str, const wxString& delimiter)
{
    wxArrayString result;
    wxStringTokenizer tokenizer(str, delimiter);
    
    while (tokenizer.HasMoreTokens()) {
        result.Add(tokenizer.GetNextToken());
    }
    
    return result;
}

wxArrayString StringUtils::SplitLines(const wxString& str)
{
    return Split(str, "\n");
}

wxArrayString StringUtils::SplitWords(const wxString& str)
{
    return Split(str, " \t\n\r");
}

wxString StringUtils::Join(const wxArrayString& strings, const wxString& separator)
{
    wxString result;
    for (size_t i = 0; i < strings.GetCount(); i++) {
        if (i > 0) {
            result += separator;
        }
        result += strings[i];
    }
    return result;
}

wxString StringUtils::Join(const std::vector<wxString>& strings, const wxString& separator)
{
    wxString result;
    for (size_t i = 0; i < strings.size(); i++) {
        if (i > 0) {
            result += separator;
        }
        result += strings[i];
    }
    return result;
}

// String replacement and modification
wxString StringUtils::Replace(const wxString& str, const wxString& old_val, const wxString& new_val)
{
    wxString result = str;
    result.Replace(old_val, new_val, false); // Replace first occurrence
    return result;
}

wxString StringUtils::ReplaceAll(const wxString& str, const wxString& old_val, const wxString& new_val)
{
    wxString result = str;
    result.Replace(old_val, new_val, true); // Replace all occurrences
    return result;
}

wxString StringUtils::ReplaceIgnoreCase(const wxString& str, const wxString& old_val, const wxString& new_val)
{
    wxString result = str;
    wxString upper_str = str.Upper();
    wxString upper_old = old_val.Upper();
    
    size_t pos = upper_str.find(upper_old);
    while (pos != wxString::npos) {
        result.replace(pos, old_val.Length(), new_val);
        upper_str.replace(pos, old_val.Length(), new_val.Upper());
        pos = upper_str.find(upper_old, pos + new_val.Length());
    }
    
    return result;
}

wxString StringUtils::RemoveAll(const wxString& str, const wxString& to_remove)
{
    return ReplaceAll(str, to_remove, "");
}

wxString StringUtils::RemoveCharacters(const wxString& str, const wxString& chars_to_remove)
{
    wxString result;
    for (size_t i = 0; i < str.Length(); i++) {
        if (chars_to_remove.find(str[i]) == wxString::npos) {
            result += str[i];
        }
    }
    return result;
}

wxString StringUtils::KeepOnly(const wxString& str, const wxString& chars_to_keep)
{
    wxString result;
    for (size_t i = 0; i < str.Length(); i++) {
        if (chars_to_keep.find(str[i]) != wxString::npos) {
            result += str[i];
        }
    }
    return result;
}

// String padding and alignment
wxString StringUtils::PadLeft(const wxString& str, size_t width, wxChar pad_char)
{
    if (str.Length() >= width) {
        return str;
    }
    
    return wxString(pad_char, width - str.Length()) + str;
}

wxString StringUtils::PadRight(const wxString& str, size_t width, wxChar pad_char)
{
    if (str.Length() >= width) {
        return str;
    }
    
    return str + wxString(pad_char, width - str.Length());
}

wxString StringUtils::PadCenter(const wxString& str, size_t width, wxChar pad_char)
{
    if (str.Length() >= width) {
        return str;
    }
    
    size_t total_padding = width - str.Length();
    size_t left_padding = total_padding / 2;
    size_t right_padding = total_padding - left_padding;
    
    return wxString(pad_char, left_padding) + str + wxString(pad_char, right_padding);
}

wxString StringUtils::Repeat(const wxString& str, size_t count)
{
    wxString result;
    for (size_t i = 0; i < count; i++) {
        result += str;
    }
    return result;
}

// String searching and indexing
int StringUtils::FindFirst(const wxString& str, const wxString& substring)
{
    return str.find(substring);
}

int StringUtils::FindLast(const wxString& str, const wxString& substring)
{
    return str.rfind(substring);
}

int StringUtils::FindIgnoreCase(const wxString& str, const wxString& substring)
{
    return str.Upper().find(substring.Upper());
}

int StringUtils::CountOccurrences(const wxString& str, const wxString& substring)
{
    if (substring.IsEmpty()) return 0;
    
    int count = 0;
    size_t pos = 0;
    
    while ((pos = str.find(substring, pos)) != wxString::npos) {
        count++;
        pos += substring.Length();
    }
    
    return count;
}

int StringUtils::CountOccurrences(const wxString& str, wxChar ch)
{
    int count = 0;
    for (size_t i = 0; i < str.Length(); i++) {
        if (str[i] == ch) {
            count++;
        }
    }
    return count;
}

// String extraction
wxString StringUtils::Left(const wxString& str, size_t count)
{
    return str.Left(count);
}

wxString StringUtils::Right(const wxString& str, size_t count)
{
    return str.Right(count);
}

wxString StringUtils::Mid(const wxString& str, size_t start, size_t length)
{
    return str.Mid(start, length);
}

wxString StringUtils::Between(const wxString& str, const wxString& start_delimiter, const wxString& end_delimiter)
{
    size_t start_pos = str.find(start_delimiter);
    if (start_pos == wxString::npos) {
        return wxEmptyString;
    }
    
    start_pos += start_delimiter.Length();
    size_t end_pos = str.find(end_delimiter, start_pos);
    if (end_pos == wxString::npos) {
        return wxEmptyString;
    }
    
    return str.Mid(start_pos, end_pos - start_pos);
}

wxString StringUtils::BeforeFirst(const wxString& str, const wxString& delimiter)
{
    return str.BeforeFirst(delimiter[0]);
}

wxString StringUtils::AfterFirst(const wxString& str, const wxString& delimiter)
{
    return str.AfterFirst(delimiter[0]);
}

wxString StringUtils::BeforeLast(const wxString& str, const wxString& delimiter)
{
    return str.BeforeLast(delimiter[0]);
}

wxString StringUtils::AfterLast(const wxString& str, const wxString& delimiter)
{
    return str.AfterLast(delimiter[0]);
}

// String validation and formatting
bool StringUtils::IsValidEmail(const wxString& email)
{
    wxRegEx email_regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return email_regex.Matches(email);
}

bool StringUtils::IsValidUrl(const wxString& url)
{
    return url.StartsWith("http://") || url.StartsWith("https://") || url.StartsWith("ftp://");
}

bool StringUtils::IsValidFilename(const wxString& filename)
{
    if (filename.IsEmpty()) return false;
    
    wxString invalid_chars = "<>:\"/\\|?*";
    for (size_t i = 0; i < filename.Length(); i++) {
        if (invalid_chars.find(filename[i]) != wxString::npos || filename[i] < 32) {
            return false;
        }
    }
    return true;
}

wxString StringUtils::SanitizeFilename(const wxString& filename)
{
    wxString result = filename;
    wxString invalid_chars = "<>:\"/\\|?*";
    
    for (size_t i = 0; i < result.Length(); i++) {
        if (invalid_chars.find(result[i]) != wxString::npos || result[i] < 32) {
            result[i] = '_';
        }
    }
    
    return Trim(result);
}

wxString StringUtils::EscapeSpecialChars(const wxString& str)
{
    wxString escaped = str;
    escaped.Replace("\\", "\\\\");
    escaped.Replace("\"", "\\\"");
    escaped.Replace("'", "\\'");
    escaped.Replace("\n", "\\n");
    escaped.Replace("\r", "\\r");
    escaped.Replace("\t", "\\t");
    return escaped;
}

wxString StringUtils::UnescapeSpecialChars(const wxString& str)
{
    wxString unescaped = str;
    unescaped.Replace("\\\\", "\\");
    unescaped.Replace("\\\"", "\"");
    unescaped.Replace("\\'", "'");
    unescaped.Replace("\\n", "\n");
    unescaped.Replace("\\r", "\r");
    unescaped.Replace("\\t", "\t");
    return unescaped;
}

// String conversion
long StringUtils::ToLong(const wxString& str, long default_value)
{
    long value;
    if (str.ToLong(&value)) {
        return value;
    }
    return default_value;
}

double StringUtils::ToDouble(const wxString& str, double default_value)
{
    double value;
    if (str.ToDouble(&value)) {
        return value;
    }
    return default_value;
}

bool StringUtils::ToBool(const wxString& str, bool default_value)
{
    wxString lower = str.Lower();
    if (lower == "true" || lower == "1" || lower == "yes" || lower == "on") {
        return true;
    } else if (lower == "false" || lower == "0" || lower == "no" || lower == "off") {
        return false;
    }
    return default_value;
}

wxString StringUtils::FromLong(long value)
{
    return wxString::Format("%ld", value);
}

wxString StringUtils::FromDouble(double value, int precision)
{
    if (precision >= 0) {
        return wxString::Format("%.*f", precision, value);
    } else {
        return wxString::Format("%g", value);
    }
}

wxString StringUtils::FromBool(bool value)
{
    return value ? "true" : "false";
}

// String formatting
wxString StringUtils::FormatSize(long long bytes)
{
    const long long KB = 1024;
    const long long MB = KB * 1024;
    const long long GB = MB * 1024;
    const long long TB = GB * 1024;

    if (bytes < KB) {
        return wxString::Format("%lld B", bytes);
    } else if (bytes < MB) {
        return wxString::Format("%.1f KB", (double)bytes / KB);
    } else if (bytes < GB) {
        return wxString::Format("%.1f MB", (double)bytes / MB);
    } else if (bytes < TB) {
        return wxString::Format("%.1f GB", (double)bytes / GB);
    } else {
        return wxString::Format("%.1f TB", (double)bytes / TB);
    }
}

wxString StringUtils::FormatNumber(long long number)
{
    wxString result = FromLong(number);
    
    // Add thousand separators
    int pos = result.Length() - 3;
    while (pos > 0) {
        result.insert(pos, ",");
        pos -= 3;
    }
    
    return result;
}

wxString StringUtils::FormatPercent(double percent, int decimals)
{
    return wxString::Format("%.*f%%", decimals, percent);
}

// Basic implementations for other functions
wxString StringUtils::Reverse(const wxString& str)
{
    wxString result = str;
    std::reverse(result.begin(), result.end());
    return result;
}

wxString StringUtils::GenerateRandom(size_t length, const wxString& charset)
{
    if (length == 0 || charset.IsEmpty()) {
        return wxEmptyString;
    }
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, charset.Length() - 1);
    
    wxString result;
    for (size_t i = 0; i < length; i++) {
        result += charset[dis(gen)];
    }
    
    return result;
}

// Helper functions
bool StringUtils::IsWhitespaceChar(wxChar ch)
{
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

// Stub implementations for complex functions
wxString StringUtils::EscapeXml(const wxString& str) { return str; }
wxString StringUtils::UnescapeXml(const wxString& str) { return str; }
wxString StringUtils::EscapeJson(const wxString& str) { return str; }
wxString StringUtils::UnescapeJson(const wxString& str) { return str; }
wxString StringUtils::EscapeRegex(const wxString& str) { return str; }
wxString StringUtils::UrlEncode(const wxString& str) { return str; }
wxString StringUtils::UrlDecode(const wxString& str) { return str; }
int StringUtils::Compare(const wxString& str1, const wxString& str2) { return str1.Cmp(str2); }
int StringUtils::CompareIgnoreCase(const wxString& str1, const wxString& str2) { return str1.CmpNoCase(str2); }
int StringUtils::CompareNatural(const wxString& str1, const wxString& str2) { return str1.Cmp(str2); }
bool StringUtils::Equals(const wxString& str1, const wxString& str2) { return str1 == str2; }
bool StringUtils::EqualsIgnoreCase(const wxString& str1, const wxString& str2) { return str1.CmpNoCase(str2) == 0; }
wxString StringUtils::FormatList(const wxArrayString& items, const wxString& separator, const wxString& last_separator) { return Join(items, separator); }
wxString StringUtils::Shuffle(const wxString& str) { return str; }
wxString StringUtils::ToBase64(const wxString& str) { return wxBase64Encode(str.c_str(), str.Length()); }
wxString StringUtils::FromBase64(const wxString& base64_str) { 
    wxMemoryBuffer buf = wxBase64Decode(base64_str);
    return wxString((const char*)buf.GetData(), wxConvUTF8, buf.GetDataLen());
}
double StringUtils::CalculateSimilarity(const wxString& str1, const wxString& str2) { return 0.0; }
int StringUtils::LevenshteinDistance(const wxString& str1, const wxString& str2) { return 0; }
wxString StringUtils::GetCommonPrefix(const wxString& str1, const wxString& str2) { return wxEmptyString; }
wxString StringUtils::GetCommonSuffix(const wxString& str1, const wxString& str2) { return wxEmptyString; }
wxArrayString StringUtils::WrapLines(const wxString& text, size_t line_width) { return SplitLines(text); }
wxString StringUtils::UnwrapLines(const wxString& text) { return ReplaceAll(text, "\n", " "); }
wxString StringUtils::IndentLines(const wxString& text, const wxString& indent) { return text; }
wxString StringUtils::UnindentLines(const wxString& text) { return text; }
wxString StringUtils::GetMD5Hash(const wxString& str) { return wxEmptyString; }
wxString StringUtils::GetSHA1Hash(const wxString& str) { return wxEmptyString; }
wxUint32 StringUtils::GetCRC32(const wxString& str) { return 0; }
bool StringUtils::IsValidEmailChar(wxChar ch) { return wxIsalnum(ch) || ch == '@' || ch == '.' || ch == '_' || ch == '-'; }
bool StringUtils::IsValidFilenameChar(wxChar ch) { return ch >= 32 && wxString("<>:\"/\\|?*").find(ch) == wxString::npos; }
wxString StringUtils::GenerateRandomString(size_t length, const wxString& charset) { return GenerateRandom(length, charset); }
double StringUtils::JaccardSimilarity(const wxString& str1, const wxString& str2) { return 0.0; }

}