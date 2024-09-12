import subprocess
import time
import functools


def run_script(commands):
    process = subprocess.Popen(
        ["./db", "test.db"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        encoding="utf-8",
        errors="replace",
    )

    # Send commands to the database program
    for command in commands:
        process.stdin.write(command + "\n")
    process.stdin.close()

    # Get the output from the database program
    output = process.stdout.read()
    process.stdout.close()

    process.wait()
    return output


def assert_output(results, expected):
    result_lines = results.split("\n")

    assert len(result_lines) == len(expected), f"Expected {len(expected)} lines, \n\t\t but got {len(result_lines)}"

    for i, (expected, actual) in enumerate(zip(expected, result_lines)):
        assert actual == expected, f"Test failed at line {i + 1}: \n\t\tExpected '{expected}',\n\t\t but got '{actual}'"


def test_timer(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        subprocess.run(["rm", "-rf", "test.db"])
        start_time = time.time()
        results = func(*args, **kwargs)
        end_time = time.time()
        print(f"{func.__name__:30} passed! Time: {end_time - start_time:.4f} seconds")
        return results

    return wrapper


@test_timer
def test_insert_and_select():
    commands = ["insert 1 user1 person1@example.com", "select", ".exit"]

    expected_results = [
        "db > Executed.",
        "db > (1, user1, person1@example.com)",
        "Executed.",
        "db > ",
    ]

    results = run_script(commands)

    assert_output(results, expected_results)


@test_timer
def test_table_full():
    commands = [f"insert {i} user{i} person{i}@example.com" for i in range(1, 1302)]
    commands.append(".exit")

    results = run_script(commands)
    result_lines = results.split("\n")

    expected_line = "db > Error: Table full."

    assert result_lines[-2] == expected_line, f"Expected '{expected_line}', but got '{result_lines[-2]}'"


@test_timer
def test_max_length_strings():
    long_username = "a" * 32
    long_email = "a" * 255
    commands = [
        f"insert 1 {long_username} {long_email}",
        "select",
        ".exit",
    ]

    expected_results = [
        "db > Executed.",
        f"db > (1, {long_username}, {long_email})",
        "Executed.",
        "db > ",
    ]

    results = run_script(commands)

    assert_output(results, expected_results)


@test_timer
def test_strings_too_long():
    long_username = "a" * 33
    long_email = "a" * 256
    commands = [
        f"insert 1 {long_username} {long_email}",
        "select",
        ".exit",
    ]

    expected_results = [
        "db > String is too long.",
        "db > Executed.",
        "db > ",
    ]

    results = run_script(commands)

    assert_output(results, expected_results)


@test_timer
def test_negative_id():
    commands = [
        "insert -1 cstack foo@bar.com",
        "select",
        ".exit",
    ]

    expected_results = [
        "db > ID must be positive.",
        "db > Executed.",
        "db > ",
    ]

    results = run_script(commands)

    assert_output(results, expected_results)


@test_timer
def all_test():
    test_insert_and_select()
    test_table_full()
    test_max_length_strings()
    test_strings_too_long()
    test_negative_id()


if __name__ == "__main__":
    all_test()
