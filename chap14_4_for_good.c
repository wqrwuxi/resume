 while (feof(fp) == 0)
    {
        fscanf(fp, "%d", &player_number);
        fscanf(fp, "%s", &player.first_name);
        fscanf(fp, "%s", &player.last_name);
        fscanf(fp, "%d", &player.play_times);
        fscanf(fp, "%d", &player.hit_times);
        fscanf(fp, "%d", &player.run_bar_times);
        fscanf(fp, "%d", &player.rbi);
        if (players[player_number].first_name[0] == '\0')
        {
            players[player_number] = player;
        }
        else
        {
            players[player_number].play_times += player.play_times;
            players[player_number].hit_times += player.hit_times;
            players[player_number].run_bar_times += player.run_bar_times;
            players[player_number].rbi += player.rbi;
        }
    }