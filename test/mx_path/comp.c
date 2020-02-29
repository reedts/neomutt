/**
 * @file
 * Test code for the Compressed MxOps Path functions
 *
 * @authors
 * Copyright (C) 2020 Richard Russon <rich@flatcap.org>
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define TEST_NO_MAIN
#include "acutest.h"
#include "config.h"
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include "mutt/lib.h"
#include "core/lib.h"
#include "common.h"
#include "compress/path.h"

bool mutt_comp_can_read(const char *path)
{
  static const char *suffix = ".gz";
  size_t len = strlen(path);

  return (strcmp(path + len - 3, suffix) == 0);
}

void test_comp_path2_canon(void)
{
  // clang-format off
  static const struct TestValue tests[] = {
    { "%s/compress/apple.gz",         "%s/compress/apple.gz",  0 }, // Real path
    { "%s/compress/symlink/apple.gz", "%s/compress/apple.gz",  0 }, // Symlink
    { "%s/compress/missing",          NULL,                   -1 }, // Missing
  };
  // clang-format on

  char first[256] = { 0 };
  char second[256] = { 0 };

  struct Path path = { 0 };
  int rc;
  for (size_t i = 0; i < mutt_array_size(tests); i++)
  {
    test_gen_path(first, sizeof(first), tests[i].first);
    test_gen_path(second, sizeof(second), tests[i].second);

    path.orig = first;
    TEST_CASE(path.orig);
    path.type = MUTT_COMPRESSED;
    path.flags = MPATH_RESOLVED | MPATH_TIDY;

    rc = comp_path2_canon(&path);
    TEST_CHECK(rc == tests[i].retval);
    if (rc == 0)
    {
      TEST_CHECK(path.flags & MPATH_CANONICAL);
      TEST_CHECK(path.canon != NULL);
      TEST_CHECK(mutt_str_strcmp(path.canon, second) == 0);
    }
    FREE(&path.canon);
  }
}

void test_comp_path2_compare(void)
{
  // clang-format off
  static const struct TestValue tests[] = {
    { "%s/compress/apple.gz",  "%s/compress/apple.gz",   0 }, // Match
    { "%s/compress/apple.gz",  "%s/compress/orange.gz", -1 }, // Differ
    { "%s/compress/orange.gz", "%s/compress/apple.gz",   1 }, // Differ
  };
  // clang-format on

  char first[256] = { 0 };
  char second[256] = { 0 };

  struct Path path1 = {
    .type = MUTT_COMPRESSED,
    .flags = MPATH_RESOLVED | MPATH_TIDY | MPATH_CANONICAL,
  };
  struct Path path2 = {
    .type = MUTT_COMPRESSED,
    .flags = MPATH_RESOLVED | MPATH_TIDY | MPATH_CANONICAL,
  };

  int rc;
  for (size_t i = 0; i < mutt_array_size(tests); i++)
  {
    test_gen_path(first, sizeof(first), tests[i].first);
    test_gen_path(second, sizeof(second), tests[i].second);

    path1.canon = first;
    TEST_CASE(path1.canon);

    path2.canon = second;
    TEST_CASE(path2.canon);

    rc = comp_path2_compare(&path1, &path2);
    TEST_CHECK(rc == tests[i].retval);
  }
}

void test_comp_path2_parent(void)
{
  // clang-format off
  static struct TestValue tests[] = {
    { "%s/compress/apple.gz", NULL, -1 },
  };
  // clang-format on

  char first[256] = { 0 };
  char second[256] = { 0 };

  struct Path path = {
    .type = MUTT_COMPRESSED,
    .flags = MPATH_RESOLVED | MPATH_TIDY,
  };

  struct Path *parent = NULL;
  int rc;
  for (size_t i = 0; i < mutt_array_size(tests); i++)
  {
    test_gen_path(first, sizeof(first), tests[i].first);
    test_gen_path(second, sizeof(second), tests[i].second);

    path.orig = first;
    TEST_CASE(path.orig);

    rc = comp_path2_parent(&path, &parent);
    TEST_CHECK(rc == tests[i].retval);
    TEST_CHECK(mutt_str_strcmp(parent ? parent->orig : NULL, second) == 0);
  }
}

void test_comp_path2_pretty(void)
{
  // clang-format off
  static struct TestValue tests[] = {
    { "%s/compress/apple.gz",         "+compress/apple.gz",         1 },
    { "%s/compress/symlink/apple.gz", "+compress/symlink/apple.gz", 1 },
  };
  // clang-format on

  char first[256] = { 0 };
  char second[256] = { 0 };
  char folder[256] = { 0 };

  test_gen_path(folder, sizeof(folder), "%s");

  struct Path path = {
    .type = MUTT_COMPRESSED,
    .flags = MPATH_RESOLVED | MPATH_TIDY,
  };

  char *pretty = NULL;
  int rc;
  for (size_t i = 0; i < mutt_array_size(tests); i++)
  {
    test_gen_path(first, sizeof(first), tests[i].first);
    test_gen_path(second, sizeof(second), tests[i].second);

    path.orig = first;
    TEST_CASE(path.orig);

    rc = comp_path2_pretty(&path, folder, &pretty);
    TEST_CHECK(rc == tests[i].retval);
    if (rc >= 0)
    {
      TEST_CHECK(pretty != NULL);
      TEST_CHECK(mutt_str_strcmp(pretty, second) == 0);
    }
    FREE(&pretty);
  }

  test_gen_path(first, sizeof(first), tests[0].first);
  test_gen_dir(second, sizeof(second), "~/%s/compress/apple.gz");
  path.orig = first;
  HomeDir = mutt_str_getenv("HOME");
  rc = comp_path2_pretty(&path, "nowhere", &pretty);
  TEST_CHECK(rc == 1);
  TEST_CHECK(pretty != NULL);
  TEST_CHECK(mutt_str_strcmp(pretty, second) == 0);
  FREE(&pretty);

  test_gen_path(first, sizeof(first), tests[0].first);
  test_gen_path(second, sizeof(second), tests[0].first);
  path.orig = first;
  HomeDir = "/home/another";
  rc = comp_path2_pretty(&path, "nowhere", &pretty);
  TEST_CHECK(rc == 0);
  TEST_CHECK(pretty != NULL);
  TEST_CHECK(mutt_str_strcmp(pretty, second) == 0);
  FREE(&pretty);
}

void test_comp_path2_probe(void)
{
  // clang-format off
  static const struct TestValue tests[] = {
    { "%s/compress/apple.gz",  NULL,  0 }, // Accepted
    { "%s/compress/banana.gz", NULL, -1 }, // Directory
    { "%s/compress/cherry.xz", NULL, -1 }, // Not accepted
    { "%s/compress/damson.gz", NULL, -1 }, // Missing
  };
  // clang-format on

  char first[256] = { 0 };

  struct Path path = { 0 };
  struct stat st;
  int rc;
  for (size_t i = 0; i < mutt_array_size(tests); i++)
  {
    test_gen_path(first, sizeof(first), tests[i].first);

    path.orig = first;
    TEST_CASE(path.orig);
    path.type = MUTT_UNKNOWN;
    path.flags = MPATH_RESOLVED | MPATH_TIDY;
    memset(&st, 0, sizeof(st));
    stat(path.orig, &st);
    rc = comp_path2_probe(&path, &st);
    TEST_CHECK(rc == tests[i].retval);
    if (rc == 0)
    {
      TEST_CHECK(path.type > MUTT_UNKNOWN);
    }
  }
}

void test_comp_path2_tidy(void)
{
  // clang-format off
  static const struct TestValue tests[] = {
    { "%s/./compress/../compress///apple.gz", "%s/compress/apple.gz", 0 },
  };
  // clang-format on

  char first[256] = { 0 };
  char second[256] = { 0 };

  struct Path path = {
    .type = MUTT_COMPRESSED,
    .flags = MPATH_RESOLVED,
  };

  int rc;
  for (size_t i = 0; i < mutt_array_size(tests); i++)
  {
    test_gen_path(first, sizeof(first), tests[i].first);
    test_gen_path(second, sizeof(second), tests[i].second);

    path.orig = mutt_str_strdup(first);
    rc = comp_path2_tidy(&path);
    TEST_CHECK(rc == 0);
    TEST_CHECK(path.orig != NULL);
    TEST_CHECK(path.flags & MPATH_TIDY);
    TEST_CHECK(strcmp(path.orig, second) == 0);
    FREE(&path.orig);
  }
}
