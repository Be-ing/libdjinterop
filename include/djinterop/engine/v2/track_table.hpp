/*
    This file is part of libdjinterop.

    libdjinterop is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libdjinterop is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with libdjinterop.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#if __cplusplus < 201703L
#error This library needs at least a C++17 compliant compiler
#endif

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include <djinterop/config.hpp>
#include <djinterop/engine/v2/beat_data_blob.hpp>
#include <djinterop/engine/v2/loops_blob.hpp>
#include <djinterop/engine/v2/overview_waveform_data_blob.hpp>
#include <djinterop/engine/v2/quick_cues_blob.hpp>
#include <djinterop/engine/v2/track_data_blob.hpp>
#include <djinterop/optional.hpp>

namespace djinterop::engine::v2
{
struct engine_library_context;

/// Thrown when the id on a track row is in an erroneous state for a given
/// operation.
struct DJINTEROP_PUBLIC track_row_id_error : public std::runtime_error
{
public:
    explicit track_row_id_error(const std::string& what_arg) noexcept
        : runtime_error{what_arg}
    {
    }
};

/// Represents a row in the `Track` table.
struct DJINTEROP_PUBLIC track_row
{
    /// Auto-generated id column.
    ///
    /// A value of `0` can be used to indicate a track row that is not yet
    /// persisted in the table, e.g. when adding a new row.
    int64_t id;

    /// `playOrder` column.
    int64_t play_order;

    /// `length` column, representing the approximate length, in seconds.
    int64_t length;

    /// `bpm` column, representing the approximate BPM.
    int64_t bpm;

    /// `year` column.
    stdx::optional<int64_t> year;

    /// `path` column.
    ///
    /// Represents the relative path from the location of the Engine library to
    /// the music file underlying the track.
    std::string path;

    /// `filename` column, representing the name of the file underlying the
    /// track.
    std::string filename;

    /// `bitrate` column.
    int64_t bitrate;

    /// `bpmAnalyzed` column, representing the BPM as determined from track
    /// analysis.
    double bpm_analyzed;

    /// `albumArtId` column.
    ///
    /// The id may be set to zero if there is no album art.
    int64_t album_art_id;

    /// `file_bytes` column, representing the size of the file underlying the
    /// track on disk, in bytes.
    int64_t file_bytes;

    /// `title` column.
    stdx::optional<std::string> title;

    /// `artist` column.
    stdx::optional<std::string> artist;

    /// `album column.
    stdx::optional<std::string> album;

    /// `genre` column.
    stdx::optional<std::string> genre;

    /// `comment` column.
    stdx::optional<std::string> comment;

    /// `label` column.
    stdx::optional<std::string> label;

    /// `composer` column.
    stdx::optional<std::string> composer;

    /// `remixer` column.
    stdx::optional<std::string> remixer;

    /// `key` column, as an integer between 0 and 23.
    ///
    /// The values align with those in the `djinterop::musical_key` enum.
    int64_t key;

    /// `rating` column.
    ///
    /// The value is set to zero if there is no rating.
    int64_t rating;

    /// `albumArt` column.
    stdx::optional<std::string> album_art;

    /// `timeLastPlayed` column.
    stdx::optional<std::chrono::system_clock::time_point> time_last_played;

    /// `isPlayed` column, indicating whether the track has ever been played.
    bool is_played;

    /// `fileType` column, representing the format of the file underlying the
    /// track.
    std::string file_type;

    /// `isAnalyzed` column, indicating whether the track has been analysed.
    bool is_analyzed;

    /// `dateCreated` column.
    stdx::optional<std::chrono::system_clock::time_point> date_created;

    /// `dateAdded` column, representing the time at which the track was added
    /// to the database.
    stdx::optional<std::chrono::system_clock::time_point> date_added;

    /// `isAvailable` column, indicating if the file underpinning the track
    /// entry is available.
    bool is_available;

    /// `isMetadataOfPackedTrackChanged` column.
    bool is_metadata_of_packed_track_changed;

    /// `isPerformanceDataOfPackedTrackChanged` column.
    ///
    /// Note that the column is misspelled as "perfomance" in a real database.
    bool is_performance_data_of_packed_track_changed;

    /// `playedIndicator` column.
    ///
    /// Random number matching the equivalent field in the `Information` table,
    /// if this track is the most recent to be played.
    stdx::optional<int64_t> played_indicator;

    /// `isMetadataImported` column.
    bool is_metadata_imported;

    /// `pdbImportKey` column.
    int64_t pdb_import_key;

    /// `streamingSource` column.
    stdx::optional<std::string> streaming_source;

    /// `uri` column.
    stdx::optional<std::string> uri;

    /// `isBeatGridLocked` column.
    bool is_beat_grid_locked;

    /// `originDatabaseUuid` column, representing the UUID of the database from
    /// which the track was originally imported.
    ///
    /// If the track has not been imported from another database, this field is
    /// set to the UUID of the current database.
    std::string origin_database_uuid;

    /// `originTrackId` column, representing the ID of the track in the
    /// original database from which it was imported.
    ///
    /// If the track has not been imported from another database, this field is
    /// set to the id of the track in the current database.
    int64_t origin_track_id;

    /// `trackData` column.
    track_data_blob track_data;

    /// `overviewWaveFormData` column.
    overview_waveform_data_blob overview_waveform_data;

    /// `beatData` column.
    beat_data_blob beat_data;

    /// `quickCues` column.
    quick_cues_blob quick_cues;

    /// `loops` column.
    loops_blob loops;

    /// `thirdPartySourceId` column.
    stdx::optional<int64_t> third_party_source_id;

    /// `streamingFlags` column.
    int64_t streaming_flags;

    /// `explicitLyrics` column.
    bool explicit_lyrics;
};

/// Represents the `Track` table in an Engine v2 database.
class DJINTEROP_PUBLIC track_table
{
public:
    /// Construct an instance of the class using an Engine library context.
    ///
    /// \param context Engine library context.
    explicit track_table(std::shared_ptr<engine_library_context> context);

    /// Add a track row to the table.
    ///
    /// \param row Track row to add.
    /// \return Returns the id of the newly-added track row.
    /// \throws track_row_id_error If the row already has an id.
    int64_t add(const track_row& row);

    /// Get the ids of all tracks in the table.
    ///
    /// \return Returns a list of all ids.
    [[nodiscard]] std::vector<int64_t> all_ids() const;

    /// Get an entire track row.
    ///
    /// \param id Id of track.
    /// \return Returns a track row, or none if not found.
    stdx::optional<track_row> get(int64_t id);

    /// Get the `playOrder` column for a given track.
    int64_t get_play_order(int64_t id);

    /// Set the `playOrder` column for a given track.
    void set_play_order(int64_t id, int64_t play_order);

    /// Get the `length` column for a given track, representing the approximate
    /// length, in seconds.
    int64_t get_length(int64_t id);

    /// Set the `length` column for a given track, representing the approximate
    /// length, in seconds.
    void set_length(int64_t id, int64_t length);

    /// Get the `bpm` column for a given track, representing the approximate
    /// BPM.
    int64_t get_bpm(int64_t id);

    /// Set the `bpm` column for a given track, representing the approximate
    /// BPM.
    void set_bpm(int64_t id, int64_t bpm);

    /// Get the `year` column for a given track.
    stdx::optional<int64_t> get_year(int64_t id);

    /// Set the `year` column for a given track.
    void set_year(int64_t id, const stdx::optional<int64_t>& year);

    /// Get the `path` column for a given track.
    ///
    /// Represents the relative path from the location of the Engine library to
    /// the music file underlying the track.
    std::string get_path(int64_t id);

    /// Set the `path` column for a given track.
    ///
    /// Represents the relative path from the location of the Engine library to
    /// the music file underlying the track.
    void set_path(int64_t id, const std::string& path);

    /// Get the `filename` column for a given track, representing the name of
    /// the file underlying the track.
    std::string get_filename(int64_t id);

    /// Set the `filename` column for a given track, representing the name of
    /// the file underlying the track.
    void set_filename(int64_t id, const std::string& filename);

    /// Get the `bitrate` column for a given track.
    int64_t get_bitrate(int64_t id);

    /// Set the `bitrate` column for a given track.
    void set_bitrate(int64_t id, int64_t bitrate);

    /// Get the `bpmAnalyzed` column for a given track, representing the BPM as
    /// determined from track analysis.
    double get_bpm_analyzed(int64_t id);

    /// Set the `bpmAnalyzed` column for a given track, representing the BPM as
    /// determined from track analysis.
    void set_bpm_analyzed(int64_t id, double bpm_analyzed);

    /// Get the `albumArtId` column for a given track.
    ///
    /// The id may be set to zero if there is no album art.
    int64_t get_album_art_id(int64_t id);

    /// Set the `albumArtId` column for a given track.
    ///
    /// The id may be set to zero if there is no album art.
    void set_album_art_id(int64_t id, int64_t album_art_id);

    /// Get the `file_bytes` column for a given track, representing the size of
    /// the file underlying the track on disk, in bytes.
    int64_t get_file_bytes(int64_t id);

    /// Set the `file_bytes` column for a given track, representing the size of
    /// the file underlying the track on disk, in bytes.
    void set_file_bytes(int64_t id, int64_t file_bytes);

    /// Get the `title` column for a given track.
    stdx::optional<std::string> get_title(int64_t id);

    /// Set the `title` column for a given track.
    void set_title(int64_t id, const stdx::optional<std::string>& title);

    /// Get the `artist` column for a given track.
    stdx::optional<std::string> get_artist(int64_t id);

    /// Set the `artist` column for a given track.
    void set_artist(int64_t id, const stdx::optional<std::string>& artist);

    /// Get the `album` column for a given track.
    stdx::optional<std::string> get_album(int64_t id);

    /// Set the `album` column for a given track.
    void set_album(int64_t id, const stdx::optional<std::string>& album);

    /// Get the `genre` column for a given track.
    stdx::optional<std::string> get_genre(int64_t id);

    /// Set the `genre` column for a given track.
    void set_genre(int64_t id, const stdx::optional<std::string>& genre);

    /// Get the `comment` column for a given track.
    stdx::optional<std::string> get_comment(int64_t id);

    /// Set the `comment` column for a given track.
    void set_comment(int64_t id, const stdx::optional<std::string>& comment);

    /// Get the `label` column for a given track.
    stdx::optional<std::string> get_label(int64_t id);

    /// Set the `label` column for a given track.
    void set_label(int64_t id, const stdx::optional<std::string>& label);

    /// Get the `composer` column for a given track.
    stdx::optional<std::string> get_composer(int64_t id);

    /// Set the `composer` column for a given track.
    void set_composer(int64_t id, const stdx::optional<std::string>& composer);

    /// Get the `remixer` column for a given track.
    stdx::optional<std::string> get_remixer(int64_t id);

    /// Set the `remixer` column for a given track.
    void set_remixer(int64_t id, const stdx::optional<std::string>& remixer);

    /// Get the `key` column for a given track, as an integer between 0 and 23.
    ///
    /// The values align with those in the `djinterop::musical_key` enum.
    int64_t get_key(int64_t id);

    /// Set the `key` column for a given track, as an integer between 0 and 23.
    ///
    /// The values align with those in the `djinterop::musical_key` enum.
    void set_key(int64_t id, int64_t key);

    /// Get the `rating` column for a given track.
    ///
    /// The value is set to zero if there is no rating.
    int64_t get_rating(int64_t id);

    /// Set the `rating` column for a given track.
    ///
    /// The value is set to zero if there is no rating.
    void set_rating(int64_t id, int64_t rating);

    /// Get the `albumArt` column for a given track.
    stdx::optional<std::string> get_album_art(int64_t id);

    /// Set the `albumArt` column for a given track.
    void set_album_art(
        int64_t id, const stdx::optional<std::string>& album_art);

    /// Get the `timeLastPlayed` column for a given track.
    stdx::optional<std::chrono::system_clock::time_point> get_time_last_played(
        int64_t id);

    /// Set the `timeLastPlayed` column for a given track.
    void set_time_last_played(
        int64_t id, const stdx::optional<std::chrono::system_clock::time_point>&
                        time_last_played);

    /// Get the `isPlayed` column for a given track, indicating whether the
    /// track has ever been played.
    bool get_is_played(int64_t id);

    /// Set the `isPlayed` column for a given track, indicating whether the
    /// track has ever been played.
    void set_is_played(int64_t id, bool is_played);

    /// Get the `fileType` column for a given track, representing the format of
    /// the file underlying the track.
    std::string get_file_type(int64_t id);

    /// Set the `fileType` column for a given track, representing the format of
    /// the file underlying the track.
    void set_file_type(int64_t id, const std::string& file_type);

    /// Get the `isAnalyzed` column for a given track, indicating whether the
    /// track has been analysed.
    bool get_is_analyzed(int64_t id);

    /// Set the `isAnalyzed` column for a given track, indicating whether the
    /// track has been analysed.
    void set_is_analyzed(int64_t id, bool is_analyzed);

    /// Get the `dateCreated` column for a given track.
    stdx::optional<std::chrono::system_clock::time_point> get_date_created(
        int64_t id);

    /// Set the `dateCreated` column for a given track.
    void set_date_created(
        int64_t id, const stdx::optional<std::chrono::system_clock::time_point>&
                        date_created);

    /// Get the `dateAdded` column for a given track, representing the time at
    /// which the track was added to the database.
    stdx::optional<std::chrono::system_clock::time_point> get_date_added(
        int64_t id);

    /// Set the `dateAdded` column for a given track, representing the time at
    /// which the track was added to the database.
    void set_date_added(
        int64_t id, const stdx::optional<std::chrono::system_clock::time_point>&
                        date_added);

    /// Get the `isAvailable` column for a given track, indicating if the file
    /// underlying the track entry is available.
    bool get_is_available(int64_t id);

    /// Set the `isAvailable` column for a given track, indicating if the file
    /// underlying the track entry is available.
    void set_is_available(int64_t id, bool is_available);

    /// Get the `isMetadataOfPackedTrackChanged` column for a given track.
    bool get_is_metadata_of_packed_track_changed(int64_t id);

    /// Set the `isMetadataOfPackedTrackChanged` column for a given track.
    void set_is_metadata_of_packed_track_changed(
        int64_t id, bool is_metadata_of_packed_track_changed);

    /// Get the `isPerformanceDataOfPackedTrackChanged` column for a given
    /// track.
    bool get_is_performance_data_of_packed_track_changed(int64_t id);

    /// Set the `isPerformanceDataOfPackedTrackChanged` column for a given
    /// track.
    void set_is_performance_data_of_packed_track_changed(
        int64_t id, bool is_performance_data_of_packed_track_changed);

    /// Get the `playedIndicator` column for a given track.
    ///
    /// Random number matching the equivalent field in the `Information` table,
    /// if this track is the most recent to be played.
    stdx::optional<int64_t> get_played_indicator(int64_t id);

    /// Set the `playedIndicator` column for a given track.
    ///
    /// Random number matching the equivalent field in the `Information` table,
    /// if this track is the most recent to be played.
    void set_played_indicator(
        int64_t id, const stdx::optional<int64_t>& played_indicator);

    /// Get the `isMetadataImported` column for a given track.
    bool get_is_metadata_imported(int64_t id);

    /// Set the `isMetadataImported` column for a given track.
    void set_is_metadata_imported(int64_t id, bool is_metadata_imported);

    /// Get the `pdbImportKey` column for a given track.
    int64_t get_pdb_import_key(int64_t id);

    /// Set the `pdbImportKey` column for a given track.
    void set_pdb_import_key(int64_t id, int64_t pdb_import_key);

    /// Get the `streamingSource` column for a given track.
    stdx::optional<std::string> get_streaming_source(int64_t id);

    /// Set the `streamingSource` column for a given track.
    void set_streaming_source(
        int64_t id, const stdx::optional<std::string>& streaming_source);

    /// Get the `uri` column for a given track.
    stdx::optional<std::string> get_uri(int64_t id);

    /// Set the `uri` column for a given track.
    void set_uri(int64_t id, const stdx::optional<std::string>& uri);

    /// Get the `isBeatGridLocked` column for a given track.
    bool get_is_beat_grid_locked(int64_t id);

    /// Set the `isBeatGridLocked` column for a given track.
    void set_is_beat_grid_locked(int64_t id, bool is_beat_grid_locked);

    /// Get the `originDatabaseUuid` column for a given track, representing the
    /// UUID of the database from which the track was originally imported.
    ///
    /// If the track has not been imported from another database, this field is
    /// set to the UUID of the current database.
    std::string get_origin_database_uuid(int64_t id);

    /// Set the `originDatabaseUuid` column for a given track, representing the
    /// UUID of the database from which the track was originally imported.
    ///
    /// If the track has not been imported from another database, this field is
    /// set to the UUID of the current database.
    void set_origin_database_uuid(
        int64_t id, const std::string& origin_database_uuid);

    /// Get the `originTrackId` column for a given track, representing the ID
    /// of the track in the original database from which it was imported.
    ///
    /// If the track has not been imported from another database, this field is
    /// set to the id of the track in the current database.
    int64_t get_origin_track_id(int64_t id);

    /// Set the `originTrackId` column for a given track, representing the ID
    /// of the track in the original database from which it was imported.
    ///
    /// If the track has not been imported from another database, this field is
    /// set to the id of the track in the current database.
    void set_origin_track_id(int64_t id, int64_t origin_track_id);

    /// Get the `trackData` column for a given track.
    track_data_blob get_track_data(int64_t id);

    /// Set the `trackData` column for a given track.
    void set_track_data(int64_t id, const track_data_blob& track_data);

    /// Get the `overviewWaveFormData` column for a given track.
    overview_waveform_data_blob get_overview_waveform_data(int64_t id);

    /// Set the `overviewWaveFormData` column for a given track.
    void set_overview_waveform_data(
        int64_t id, const overview_waveform_data_blob& overview_waveform_data);

    /// Get the `beatData` column for a given track.
    beat_data_blob get_beat_data(int64_t id);

    /// Set the `beatData` column for a given track.
    void set_beat_data(int64_t id, const beat_data_blob& beat_data);

    /// Get the `quickCues` column for a given track.
    quick_cues_blob get_quick_cues(int64_t id);

    /// Set the `quickCues` column for a given track.
    void set_quick_cues(int64_t id, const quick_cues_blob& quick_cues);

    /// Get the `loops` column for a given track.
    loops_blob get_loops(int64_t id);

    /// Set the `loops` column for a given track.
    void set_loops(int64_t id, const loops_blob& loops);

    /// Get the `thirdPartySourceId` column for a given track.
    stdx::optional<int64_t> get_third_party_source_id(int64_t id);

    /// Set the `thirdPartySourceId` column for a given track.
    void set_third_party_source_id(
        int64_t id, const stdx::optional<int64_t>& third_party_source_id);

    /// Get the `streamingFlags` column for a given track.
    int64_t get_streaming_flags(int64_t id);

    /// Set the `streamingFlags` column for a given track.
    void set_streaming_flags(int64_t id, int64_t streaming_flags);

    /// Get the `explicitLyrics` column for a given track.
    bool get_explicit_lyrics(int64_t id);

    /// Set the `explicitLyrics` column for a given track.
    void set_explicit_lyrics(int64_t id, bool explicit_lyrics);

    /// Update an existing track row in the table.
    ///
    /// \param row Track row to update.
    /// \throws track_row_id_error If the row has no id.
    void update(const track_row& row);

private:
    std::shared_ptr<engine_library_context> context_;
};

}  // namespace djinterop::engine::v2