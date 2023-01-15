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
#include <djinterop/track.hpp>

#define BOOST_TEST_MODULE track_test
#include <boost/test/data/test_case.hpp>
#include <boost/test/included/unit_test.hpp>

#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include <djinterop/engine/engine.hpp>
#include <djinterop/track_snapshot.hpp>

#include "../../boost_test_utils.hpp"
#include "../../track_test_utils.hpp"
#include "example_track_data.hpp"

#define STRINGIFY(x) STRINGIFY_(x)
#define STRINGIFY_(x) #x

namespace c = std::chrono;
namespace e = djinterop::engine;
namespace utf = boost::unit_test;

namespace
{
const std::vector<example_track_type> creatable_snapshot_types{
    example_track_type::minimal_1,
    example_track_type::basic_metadata_only_1,
    example_track_type::fully_analysed_1,
};

struct snapshot_type_pair
{
    example_track_type initial;
    example_track_type updated;
};

std::ostream& operator<<(std::ostream& o, const snapshot_type_pair v)
{
    o << "snapshot_type_pair(initial=" << v.initial << ", updated=" << v.updated
      << ")";
    return o;
}

const std::vector<snapshot_type_pair> updatable_snapshot_type_pairs{
    snapshot_type_pair{
        example_track_type::minimal_1,
        example_track_type::basic_metadata_only_1},
    snapshot_type_pair{
        example_track_type::minimal_1, example_track_type::fully_analysed_1},
    snapshot_type_pair{
        example_track_type::basic_metadata_only_1,
        example_track_type::minimal_1},
    snapshot_type_pair{
        example_track_type::basic_metadata_only_1,
        example_track_type::fully_analysed_1},
    snapshot_type_pair{
        example_track_type::fully_analysed_1, example_track_type::minimal_1},
    snapshot_type_pair{
        example_track_type::fully_analysed_1,
        example_track_type::basic_metadata_only_1},
};

}  // anonymous namespace

BOOST_TEST_DECORATOR(*utf::description("copy constructor, all schema versions"))
BOOST_DATA_TEST_CASE(
    ctor__supported_version_copy__copies, e::all_v2_versions, version)
{
    BOOST_TEST_CHECKPOINT("(" << version << ") Creating temporary database...");
    auto db = e::create_temporary_database(version);

    djinterop::track_snapshot snapshot{};
    populate_track_snapshot(
        example_track_type::fully_analysed_1, version, snapshot);

    do_ctor_copy_test(version, db, snapshot);
}

BOOST_TEST_DECORATOR(*utf::description("copy assignment, all schema versions"))
BOOST_DATA_TEST_CASE(
    op_copy_assign__supported_version_copy__copies, e::all_v2_versions, version)
{
    // Arrange
    BOOST_TEST_CHECKPOINT("(" << version << ") Creating temporary database...");
    auto db = e::create_temporary_database(version);

    djinterop::track_snapshot snapshot{};
    populate_track_snapshot(
        example_track_type::fully_analysed_1, version, snapshot);

    do_op_copy_assign_test(version, db, snapshot);
}

BOOST_TEST_DECORATOR(
    *utf::description("track snapshot matches created details, all schema "
                      "versions, all snapshots"))
BOOST_DATA_TEST_CASE(
    snapshot__supported_version__same,
    e::all_v2_versions* creatable_snapshot_types, version, snapshot_type)
{
    BOOST_TEST_CHECKPOINT(
        "(" << version << ", " << snapshot_type
            << ") Creating temporary database...");
    auto db = e::create_temporary_database(version);

    djinterop::track_snapshot expected{};
    populate_track_snapshot(snapshot_type, version, expected);

    do_snapshot_test(version, snapshot_type, db, expected);
}

BOOST_TEST_DECORATOR(
    *utf::description("track update with a new snapshot updates correctly, "
                      "all schema versions, all snapshot combinations"))
BOOST_DATA_TEST_CASE(
    update__supported_version__updates,
    e::all_v2_versions* updatable_snapshot_type_pairs, version,
    snapshot_type_pair)
{
    BOOST_TEST_CHECKPOINT(
        "(" << version << ", " << snapshot_type_pair
            << ") Creating temporary database...");
    auto db = e::create_temporary_database(version);

    djinterop::track_snapshot initial{}, expected{};
    populate_track_snapshot(snapshot_type_pair.initial, version, initial);
    populate_track_snapshot(snapshot_type_pair.updated, version, expected);

    do_update_test(
        version, snapshot_type_pair.initial, snapshot_type_pair.updated, db,
        initial, expected);
}

// TODO (mr-smidge): Add tests for each getter/setter