#include <cpr/cpr.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


const std::string LEAGUE_LEADERS_URL = "https://stats.nba.com/stats/leagueLeaders";

//split string on delimiter into vector<string>
std::vector<std::string> split_delim(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    int last = 0;
    int next = 0;
    while((next = s.find(delimiter, last)) != std::string::npos) {
        token = s.substr(last, next - last);
        tokens.push_back(token);
        last = next + 1;
    }

    //if last is not the end of the string and something got put into tokens
    if(last != s.length() - 1 && tokens.size() > 0) {
        token = s.substr(last, s.length() - 1);
        tokens.push_back(token);
    }

    return tokens;

}

//put substr from inside of brackets into vector<string>
std::vector<std::string> split_bracket(const std::string& s, const std::string& start, const std::string& end) {

    std::vector<std::string> tokens;
    std::string token;
    int last = s.find(start, 0) + 1;
    int next = 0;

    while((next = s.find(start, last)) != std::string::npos ) {
        token = s.substr(last, s.find(end, last) - last);
        // std::cout << token << std::endl; 
        tokens.push_back(token);
        // std::cout << tokens.back() << std::endl;
        last = next + 1;
    }

    if(last != s.length() - 1 && tokens.size() > 0 && s.find(end, last) != std::string::npos) {
        token = s.substr(last, s.find(end, last) - last);
        tokens.push_back(token);
    }

    return tokens;
}

cpr::Response api_fetch(const std::string& url) {

    cpr::Response r = cpr::Get(cpr::Url{url},
        cpr::Parameters{
            {"LeagueID", "00"}, 
            {"PerMode", "Totals"},
            {"StatCategory", "PTS"},
            {"Season", "All Time"},
            {"SeasonType", "Regular Season"},
            {"Scope", "S"},
            {"ActiveFlag", "No"},
    });

    if(r.status_code == 0){
        std::cerr <<r.error.message << std::endl;
    } else if (r.status_code >= 400) {
        std::cerr << "Error [" <<r.status_code << "] making request." << std::endl;
    } else {
        std::cout << "Request took " << r.elapsed << std::endl;
    }

    return r;
}

struct player
{
    //22 fields
    std::string player_id;
    std::string player_name;
    int games_played;
    int minutes;
    int field_goals_made;
    int field_goal_attempts;
    float field_goal_percent;
    int three_pointers_made;
    int three_pointer_attempts;
    float three_pointer_percent;
    int free_throws_made;
    int free_throw_attempts;
    float free_throw_percent;
    int offensive_rebounds;
    int defensive_rebounds;
    int total_rebounds;
    int assists;
    int steals;
    int blocks;
    int turnovers;
    int personal_fouls;
    int points;
    float assist_turnover_ratio;
    float steal_turnover_ratio;
    float effective_field_goal_percent;
    float true_shooting_percent;
    int games_played_rank;
    int minutes_rank;
    int field_goals_made_rank;
    int field_goal_attempts_rank;
    int field_goal_percent_rank;
    int offensive_rebounds_rank;
    int defensive_rebounds_rank;
    int total_rebounds_rank;
    int assists_rank;
    int steals_rank;
    int blocks_rank;
    int turnovers_rank;
    int personal_fouls_rank;
    int assist_turnover_ratio_rank;
    int steal_turnover_ratio_rank;
    float effective_field_goal_percentile;
    float true_shooting_percentile;
};

void start_chop(std::string& s) {
    const std::string start_word = "\"rowSet\":[";
    int start = s.find(start_word) + start_word.length();
    s.erase(0, start);
}


void name_prompt(std::string &name)
{
    std::cout << "Input the last name of the player you want to search for." << std::endl;
    std::cin >> name;
}

int count_char(const std::string& s, const char c) {
    int count = 0;
    for(int i = 0; i < s.length(); i++) {
        if(s[i] == c) { count++; }
    }
    return count;
}


int main() {
    cpr::Response r = api_fetch(LEAGUE_LEADERS_URL);

    std::string response_text = r.text;

    if(response_text == "") {
        std::cout << "empty response" << std::endl;
    } else {
        start_chop(response_text);

        std::vector<std::string> comma_split_data = split_delim(response_text, ",");

        std::vector<std::string> bracket_split_data = split_bracket(response_text, "[", "]");

    }
    
    
    return 0;
}