#include <cpr/cpr.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

const std::string LEAGUE_LEADERS_URL = "https://stats.nba.com/stats/leagueLeaders";

const std::string PLAYER_FIELD_NAMES[50] = {
    "player_id",
    "player_name",
    "games_played",
    "minutes_played",
    "field_goals_made",
    "field_goal_attempts",
    "field_goal_percent",
    "three_pointers_made",
    "three_pointer_attempts",
    "three_pointer_percent",
    "free_throws_made",
    "free_throw_attempts",
    "free_throw_percent",
    "offensive_rebounds",
    "defensive_rebounds",
    "total_rebounds",
    "assists",
    "steals",
    "blocks",
    "turnovers",
    "personal_fouls",
    "points",
    "assist_turnover_ratio",
    "steal_turnover_ratio",
    "effective_field_goal_percent",
    "true_shooting_percent",
    "games_played_rank",
    "minutes_rank",
    "field_goals_made_rank",
    "field_goal_attempts_rank",
    "field_goal_percent_rank",
    "three_pointers_made_rank",
    "three_pointers_attempted_rank",
    "three_pointer_percent_rank",
    "free_throws_made_rank",
    "free_throws_attempted_rank",
    "free_throw_percent_rank",
    "offensive_rebounds_rank",
    "defensive_rebounds_rank",
    "total_rebounds_rank",
    "assists_rank",
    "steals_rank",
    "blocks_rank",
    "turnovers_rank",
    "personal_fouls_rank",
    "points_rank",
    "assist_turnover_ratio_rank",
    "steal_turnover_ratio_rank",
    "effective_field_goal_PCT1",
    "true_shooting_PCT1",
};

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

    //if something got put into tokens
    if(tokens.size() > 0) {
        token = s.substr(last, s.length() - last);
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
        tokens.push_back(token);
        last = next + 1;
    }

    if(last != s.length() - 1 && tokens.size() > 0 && s.find(end, last) != std::string::npos) {
        token = s.substr(last, s.find(end, last) - last);
        tokens.push_back(token);
    }

    return tokens;
}

//https://stats.nba.com/stats/leagueLeaders?LeagueID=00&PerMode=Totals&Scope=S&Season=All%20Time&SeasonType=Regular%20Season&StatCategory=PTS
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
    std::string player_id;
    std::string player_name;
    int games_played;
    int minutes_played;
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
    int three_pointers_made_rank;
    int three_pointers_attempted_rank;
    int three_pointer_percent_rank;
    int free_throws_made_rank;
    int free_throw_attempts_rank;
    int free_throw_percent_rank;
    int offensive_rebounds_rank;
    int defensive_rebounds_rank;
    int total_rebounds_rank;
    int assists_rank;
    int steals_rank;
    int blocks_rank;
    int turnovers_rank;
    int personal_fouls_rank;
    int points_rank;
    int assist_turnover_ratio_rank;
    int steal_turnover_ratio_rank;
    float effective_field_goal_PCT1;
    float true_shooting_PCT1;
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

std::vector<std::vector<std::string>> double_process(const std::vector<std::string>& bracket_split) {
    int row_count = bracket_split.size();
    const int COLUMN_COUNT = 50;

    std::vector<std::vector<std::string> > result (
                row_count,
                std::vector<std::string>(COLUMN_COUNT));
            
    for(size_t i = 0; i < row_count; i++) {
        std::vector<std::string> target = split_delim(bracket_split[i], ",");
        if(target.size() < 50) {
            std::cout << "too small!" << std::endl;
            for(int p = 0; p < target.size(); p++) {
                std::cout << "[" << p << "] "<< PLAYER_FIELD_NAMES[p] << ": " << target[p] << " | ";
            } 
            std::cout << std::endl;
        }else if (target.size() == 51) {
            std::string temp = target[1].append(target[2]);
            target[1] = target[1].append(target[2]);
            target.erase(target.begin() + 2);
        }
        for(size_t j = 0; j < COLUMN_COUNT; j++) {
            result[i][j] = target[j];
        }
        // std::cout << double_processed_data[i][1];
    }

    return result;
}


int main() {
    cpr::Response r = api_fetch(LEAGUE_LEADERS_URL);

    std::string response_text = r.text;

    if(response_text == "") {
        std::cout << "empty response" << std::endl;
    } else {
        start_chop(response_text);
        
        

        std::vector<std::string> bracket_split_data = split_bracket(response_text, "[", "]");

        std::vector<std::vector<std::string>> double_processed_data = double_process(bracket_split_data);

        

    }
    
    
    return 0;
}